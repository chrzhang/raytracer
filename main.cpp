#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <ctime>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

#include "RGBType.hpp"
#include "PPMWriter.hpp"
#include "Vector3D.hpp"
#include "Ray3D.hpp"
#include "Camera.hpp"
#include "Color.hpp"
#include "Light.hpp"
#include "Object.hpp"
#include "Source.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"


int getForemostObjIndex(const std::vector<double> & intersections) {

    if (intersections.size() == 0) {

        return -1;

    } else if (intersections.size() == 1) {

        if (0 < *intersections.begin()) {

            return 0;

        } else {

            return -1;

        }

    } else { // Multiple intersections

        double max = *intersections.begin();

        for (std::vector<double>::const_iterator it = intersections.begin();
             it != intersections.end(); ++it) {

            if (*it > max) {
                max = *it;
            }

        }

        if (max > 0) {

            // Find the smallest positive number
            int index_of_minimum_value = -1;
            for (size_t index = 0; index < intersections.size(); ++index) {

                if (intersections[index] > 0 && intersections[index] <= max) {

                    max = intersections[index];
                    index_of_minimum_value = index;

                }

            }

            return index_of_minimum_value;

        } else {

            return -1;

        }
    }
}

Color getColorAt(const Vector3D & intersectionPoint,
                 const Vector3D & intersectionRayDirection,
                 const std::vector<Object *> & scene_objects,
                 int foremostObjIndex,
                 const std::vector<Source *> & scene_lights,
                 double accuracy,
                 double ambientLight) {

    // Get color of first object encountered
    Color foremostObjColor = (scene_objects[foremostObjIndex])->getColor();

    if (foremostObjColor.getSpecial() == 2) {

        // Checkered / Tile Floor Pattern

        int square = (int) floor(intersectionPoint.getX()) +
                     (int) floor(intersectionPoint.getZ());

        if (0 == square % 2) {
            // Black tile
            foremostObjColor.setRed(0);
            foremostObjColor.setGreen(0);
            foremostObjColor.setBlue(0);
        } else {
            // White tile
            foremostObjColor.setRed(1);
            foremostObjColor.setGreen(1);
            foremostObjColor.setBlue(1);
        }


    }

    // Finds normal at the point of intersection
    Vector3D foremostObjNormal =
        (scene_objects[foremostObjIndex])->getNormalAt(intersectionPoint);

    Color finalColor = foremostObjColor.colorScalar(ambientLight);

    // Handle reflections by checking for light bouncing off intersection
    if (foremostObjColor.getSpecial() > 0 &&
        foremostObjColor.getSpecial() <= 1) {

        // Reflection from objects with specular intensity
        double dot1 = foremostObjNormal.dotProduct(intersectionRayDirection.invert());
        Vector3D scalar1 = foremostObjNormal * dot1;
        Vector3D add1 = scalar1 + intersectionRayDirection;
        Vector3D scalar2 = add1 * 2;
        Vector3D add2 = intersectionRayDirection.invert() + scalar2;
        Vector3D reflection_direction = add2.normalize();

        Ray3D reflection_ray(intersectionPoint, reflection_direction);

        // Find first intersection of ray
        std::vector<double> reflection_intersections;

        for (size_t i = 0; i < scene_objects.size(); ++i) {
            reflection_intersections.push_back(
                scene_objects[i]->findIntersection(reflection_ray));
        }

        int indexOfForemostReflectedObject =
            getForemostObjIndex(reflection_intersections);

        if (indexOfForemostReflectedObject != -1) {
            // Reflection ray missed everything
            if (reflection_intersections[indexOfForemostReflectedObject] >
                accuracy) {

                Vector3D reflectionIntersectionPosition = intersectionPoint +
                    (reflection_direction *
                    reflection_intersections[indexOfForemostReflectedObject]);
                Vector3D reflectionIntersectionDirection = reflection_direction;
                Color reflection_intersection_color =
                    getColorAt(reflectionIntersectionPosition,
                               reflectionIntersectionDirection, scene_objects,
                               indexOfForemostReflectedObject, scene_lights,
                               accuracy, ambientLight);

                finalColor = finalColor +
                              reflection_intersection_color.colorScalar(
                                foremostObjColor.getSpecial());
            }

        }


    }

    // Loop through lights
    for (auto light_it = scene_lights.begin(); light_it != scene_lights.end();
         ++light_it) {

        // Finds direction to go from point of intersection to the light
        Vector3D lightDirection = ((*light_it)->getPosition()) +
                                  (intersectionPoint.invert());
        lightDirection = lightDirection.normalize();

        float cosine_angle = foremostObjNormal.dotProduct(lightDirection);

        if (cosine_angle > 0) {
            // Look for shadows
            bool shadowed = false;
            Vector3D distanceToLight =
                (*light_it)->getPosition() +
                (intersectionPoint.invert()); // TODO Check normalize
            float distanceToLightMagnitude = distanceToLight.getMagnitude();

            Ray3D shadow_ray(intersectionPoint,
                             ((*light_it)->getPosition() +
                             (intersectionPoint.invert())).normalize());

            std::vector<double> secondary_intersections;

            for (auto obj_it = scene_objects.begin();
                 obj_it != scene_objects.end() && !shadowed;
                 ++obj_it) {

                // Check if there's any intersections between any object and
                // the ray from the first intersection to light source
                // If so, there's an obstacle and thus a shadow

                secondary_intersections.push_back(
                    (*obj_it)->findIntersection(shadow_ray));

            }

            for (size_t c = 0; c < secondary_intersections.size(); ++c) {

                // Check secondary intersections and if the
                if (secondary_intersections[c] > accuracy) {
                    if (secondary_intersections[c] <= distanceToLightMagnitude) {
                        shadowed = true;
                    }
                }
                //break;

            }

            if (!shadowed) {

                finalColor = finalColor + (foremostObjColor *
                    (*light_it)->getColor()).colorScalar(cosine_angle);

                if (finalColor.getSpecial() > 0 &&
                    foremostObjColor.getSpecial() <= 1) { // Shiny value

                    double dot1 = foremostObjNormal.
                        dotProduct(intersectionRayDirection.invert());

                    Vector3D scalar1 = foremostObjNormal * dot1;
                    Vector3D add1 = scalar1 + intersectionRayDirection;
                    Vector3D scalar2 = add1 * 2;
                    Vector3D add2 = intersectionRayDirection.invert() + scalar2;
                    Vector3D reflectionDirection = add2.normalize();
                    double specular = reflectionDirection.
                        dotProduct(lightDirection);
                    if (specular > 0) {
                        specular = pow(specular, 10);
                        finalColor = finalColor + (*light_it)->getColor().
                            colorScalar(specular *
                                        foremostObjColor.getSpecial());
                    }

                }

            }

        }

    }

    return finalColor.clip();

}


int main() {

    clock_t start, end;

    start = clock();

    std::cout << "Rendering..." << std::endl;
    srand(time(NULL));

    size_t width = 1920;
    size_t height = 1080;
    size_t n = width * height;
    double aspectRatio = (double) width / (double) height;

    double ambientLight = 0.2;
    double accuracy = 0.000001;

    std::vector<RGBType> pixels(n, RGBType());

    size_t pixelindex;

    Vector3D X(1,0,0);
    Vector3D Y(0,1,0);
    Vector3D Z(0,0,1);

    Vector3D look_at(0,0,0);
    std::cout << "look_at" << look_at << std::endl;
    Vector3D campos(3,1.5,-4);
    std::cout << "campos" << campos << std::endl;

    Vector3D diff_btw(campos.getX() - look_at.getX(),
                      campos.getY() - look_at.getY(),
                      campos.getZ() - look_at.getZ());

    std::cout << "diff_btw" << diff_btw << std::endl;

    Vector3D camdir = diff_btw.invert().normalize();
    std::cout << "camdir" << camdir << std::endl;
    Vector3D camright = Y.crossProduct(camdir).normalize();
    std::cout << "camright" << camright << std::endl;
    Vector3D camdown = camright.crossProduct(camdir).invert();
    std::cout << "camdown" << camdown<< std::endl;

    Camera scene_cam(campos, camdir, camright, camdown);

    Color white_light(1.0, 1.0, 1.0, 0);
    Color pretty_green(0.5, 1.0, 0.5, 0.3);
    Color purple(0.5, 0.3, 0.9, 0);
    Color gray(0.5, 0.5, 0.5, 0);
    Color black(0.0, 0.0, 0.0, 0);
    Color maroon(0.5, 0.25, 0.25, 2);
    Color pink(1, 0.078, 0.576, 0);

    Vector3D light_position(-7, 5, -10);
    Light scene_light(light_position, white_light);

    // Objects in scene
    Vector3D origin(0, 0, 0);
    Sphere s(origin, 1, purple);
    Sphere s2(origin + Vector3D(0,0,3), 1, pretty_green);

    Plane p(Y, -1, maroon);

    std::vector<Object *> scene_objects;
    std::vector<Source *> scene_lights;

    scene_lights.push_back(dynamic_cast<Source *>(&scene_light));

    scene_objects.push_back(dynamic_cast<Object *>(&s));
    scene_objects.push_back(dynamic_cast<Object *>(&s2));
    scene_objects.push_back(dynamic_cast<Object *>(&p));

    /*
    std::vector<Sphere> spheres;
    for (int i = 0; i < 100; ++i) {
        Sphere ts(origin + Vector3D(rand() % 10 - 5, rand() % 10 - 5, rand() % 10 - 5), 0.2,
               Color((rand() % 10) / 10.0,
                     (rand() % 10) / 10.0,
                     (rand() % 10) / 10.0,
                     0.3));
        spheres.push_back(ts);
    }

    for (auto it = spheres.begin(); it != spheres.end(); ++it) {
        scene_objects.push_back(dynamic_cast<Object *>(&(*it)));
    }
    */

    double xamnt, yamnt;

    for (size_t x = 0; x < width; ++x) {

        for (size_t y = 0; y < height; ++y) {
            pixelindex = y * width + x;
            if (width > height) {
                xamnt = ((x + 0.5) / width) * aspectRatio -
                        (((width - height) / (double) height) / 2);
                yamnt = ((height - y) + 0.5) / height;
            } else if (height > width) {
                xamnt = (x + 0.5) / width;
                yamnt = (((height - y) + 0.5) / height) / aspectRatio -
                        (((height - width) / (double) width) / 2);
            } else {
                xamnt = (x + 0.5) / width;
                yamnt = ((height - y) + 0.5) / height;
            }

            Vector3D cam_ray_origin = scene_cam.getPosition();
            Vector3D cam_ray_direction = (camdir + (camright * (xamnt - 0.5)) +
                                         (camdown * (yamnt - 0.5))).normalize();

            Ray3D cam_ray(cam_ray_origin, cam_ray_direction);

            std::vector<double> intersections;

            // Loop through objs and see if the ray intersects any objs

            for (std::vector<Object *>::const_iterator it =
                    scene_objects.begin();
                 it != scene_objects.end(); ++it) {

                intersections.push_back((*it)->findIntersection(cam_ray));

            }

            // Index of the least positive intersection is the closest object
            int foremostObjIndex = getForemostObjIndex(intersections);

            if (foremostObjIndex == -1) {
                // Set the background to black
                pixels[pixelindex].r = 0;
                pixels[pixelindex].g = 0;
                pixels[pixelindex].b = 0;
            } else {
                if (intersections[foremostObjIndex] > accuracy) {
                    Vector3D intersectionPoint = cam_ray_origin +
                        (cam_ray_direction * intersections[foremostObjIndex]);

                    Vector3D intersectionRayDirection = cam_ray_direction;
                    //Color c = scene_objects[foremostObjIndex]->getColor();
                    // Resolve shadow
                    Color c = getColorAt(intersectionPoint,
                                         intersectionRayDirection,
                                         scene_objects,
                                         foremostObjIndex,
                                         scene_lights,
                                         accuracy,
                                         ambientLight);
                    // Get color intersection
                    pixels[pixelindex].r = c.getRed();
                    pixels[pixelindex].g = c.getGreen();
                    pixels[pixelindex].b = c.getBlue();
                }
            }

        }

    }

    PPMWriter::saveppm("scene.ppm", width, height, pixels);

    end = clock();

    float elapsed = ((float) end - (float) start) / CLOCKS_PER_SEC;

    std::cout << elapsed << " seconds." << std::endl;

    return 0;

}
