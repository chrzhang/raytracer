#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <cfloat>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

#include "RGBType.hpp"
#include "PPMWriter.hpp"
#include "PLYReader.hpp"
#include "Vector3D.hpp"
#include "Ray3D.hpp"
#include "Camera.hpp"
#include "Color.hpp"
#include "Light.hpp"
#include "Object.hpp"
#include "Source.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Triangle.hpp"
#include "Helper.hpp"
#include "Cylinder.hpp"
#include "BVH.hpp"
#include "Intersection.hpp"
#include "Grid.hpp"

Color getColorAt(const Vector3D & intersectionPoint,
                 const Vector3D & intersectionRayDirection,
                 const std::vector<Object *> & scene_objects,
                 const BVH * bounding_volume_hierarchy,
                 Object * objPtr,
                 const std::vector<Source *> & scene_lights,
                 double accuracy,
                 double ambientLight,
                 unsigned & raysFired,
                 unsigned & objsQueried) {
    ++raysFired;
    // Get color of first object encountered
    Color foremostObjColor = objPtr->getColor();
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
            foremostObjColor.setSpecial(0.2);
        }
    }
    // Finds normal at the point of intersection
    Vector3D foremostObjNormal =
        objPtr->getNormalAt(intersectionPoint);
    Color finalColor = foremostObjColor.colorScalar(ambientLight);
    // Handle reflections by checking for light bouncing off intersection
    if (foremostObjColor.getSpecial() > 0 &&
        foremostObjColor.getSpecial() <= 1) {
        // Reflection from objects with specular intensity
        double dot1 = foremostObjNormal.dotProduct(
                        intersectionRayDirection.invert());
        Vector3D scalar1 = foremostObjNormal * dot1;
        Vector3D add1 = scalar1 + intersectionRayDirection;
        Vector3D scalar2 = add1 * 2;
        Vector3D add2 = intersectionRayDirection.invert() + scalar2;
        Vector3D reflection_direction = add2.normalize();
        Ray3D reflection_ray(intersectionPoint, reflection_direction);
        // Find first intersection of ray
        /* *** Without BVH
        Intersection intersection = getIntersection(scene_objects,
                                                    reflection_ray);
        */
        Intersection intersection =
            bounding_volume_hierarchy->findIntersection(reflection_ray);
        Object * foremostReflectedObjectPtr = intersection.objPtr;
        double minDistance = intersection.distance;
        if (foremostReflectedObjectPtr != nullptr) {
            // Reflection ray missed everything
            if (minDistance >
                accuracy) {
                Vector3D reflectionIntersectionPosition = intersectionPoint +
                    (reflection_direction * minDistance);
                Vector3D reflectionIntersectionDirection = reflection_direction;
                Color reflection_intersection_color =
                    getColorAt(reflectionIntersectionPosition,
                               reflectionIntersectionDirection, scene_objects,
                               bounding_volume_hierarchy,
                               objPtr, scene_lights,
                               accuracy, ambientLight, raysFired, objsQueried);
                finalColor = finalColor +
                              reflection_intersection_color.colorScalar(
                                foremostObjColor.getSpecial());
            }
        }
    }
    // Loop through lights to see if shadows should affect
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
                (intersectionPoint.invert());
            float distanceToLightMagnitude = distanceToLight.getMagnitude();
            Ray3D shadow_ray(intersectionPoint,
                             ((*light_it)->getPosition() +
                             (intersectionPoint.invert())).normalize());
            std::vector<double> secondary_intersections;
            for (auto obj_it = scene_objects.begin();
                 obj_it != scene_objects.end() && !shadowed;
                 ++obj_it) {
                ++objsQueried;
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
                    break;
                }
            }
            if (!shadowed) { // Affect with light
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
    unsigned raysFired = 0;
    unsigned objsQueried = 0;
    Grid g(Vector3D(0, 0, 0), Vector3D(10, 10, 10), Vector3D(10, 10, 10));
    Ray3D dummyRay(Vector3D(0, 0, 0), Vector3D(1, 2, 0));
    g.findCellsIntersectedBy(dummyRay);
    start = clock();
    std::cout << "Rendering..." << std::endl;
    srand(time(NULL));
    size_t width = 192;
    size_t height = 108;
    size_t n = width * height;
    double aspectRatio = (double) width / (double) height;
    double ambientLight = 0.2;
    double accuracy = 0.000001;
    std::vector<RGBType> pixels(n, RGBType());
    int aadepth = 1; // sqrt of rays per pixel
    //double aathreshold = 0.1;
    size_t pixelindex;
    Vector3D X(1,0,0);
    Vector3D Y(0,1,0);
    Vector3D Z(0,0,1);
    Vector3D look_at(0,0,0);
    std::cout << "look_at" << look_at << std::endl;
    Vector3D campos(3,2,4);
    campos = 6 * campos;
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
    Color purple(0.5, 0.3, 0.9, 0.3);
    Color gray(0.5, 0.5, 0.5, 0);
    Color black(0.0, 0.0, 0.0, 0);
    Color maroon(0.5, 0.25, 0.25, 2);
    Color pink(1, 0.078, 0.576, 0);
    Color orange(0.94, 0.75, 0.31, 0.3);
    Vector3D light_position(3.5, 2.5, -5);
    Light scene_light(5 * light_position, white_light);
    Light scene_light2(.8 * campos, white_light);
    // Objects in scene
    Vector3D origin(0, 0, 0);
    Sphere s (origin, 1, purple);
    Sphere s2(origin + Vector3D(0,0,3), 1, pretty_green);
    Cylinder c(0.3, Color(1, 0.078, 0.576, 0.3));
    Plane p(Y, -1, maroon);
    Triangle t(Vector3D(3, 0, 0), Vector3D(0, 3, 0), Vector3D(0, 0, 3), orange);
    std::vector<Object *> scene_objects;
    std::vector<Source *> scene_lights;
    scene_lights.push_back(dynamic_cast<Source *>(&scene_light));
    scene_lights.push_back(dynamic_cast<Source *>(&scene_light2));
    scene_objects.push_back(dynamic_cast<Object *>(&s));
    scene_objects.push_back(dynamic_cast<Object *>(&s2));
    scene_objects.push_back(dynamic_cast<Object *>(&c));
    scene_objects.push_back(dynamic_cast<Object *>(&p));
    scene_objects.push_back(dynamic_cast<Object *>(&t));
    Vector3D corner1 = Vector3D(-1,-1,-1);
    Vector3D corner2 = Vector3D(-5, 1, 1);
    double c1x = corner1.getX();
    double c1y = corner1.getY();
    double c1z = corner1.getZ();
    double c2x = corner2.getX();
    double c2y = corner2.getY();
    double c2z = corner2.getZ();
    Vector3D A (c2x, c1y, c1z);
    Vector3D B (c2x, c1y, c2z);
    Vector3D C (c1x, c1y, c2z);
    Vector3D D (c2x, c2y, c1z);
    Vector3D E (c1x, c2y, c1z);
    Vector3D F (c1x, c2y, c2z);
    Triangle t1(D, A, corner1, randColor());
    Triangle t2(corner1, E, D, randColor());
    Triangle t3(corner2, B, A, randColor());
    Triangle t4(A, D, corner2, randColor());
    Triangle t5(F, C, B, randColor());
    Triangle t6(B, corner2, F, randColor());
    Triangle t7(E, corner1, C, randColor());
    Triangle t8(C, F, E, randColor());
    Triangle t9(D, E, F, randColor());
    Triangle tA(F, corner2, D, randColor());
    Triangle tB(corner1, A, B, randColor());
    Triangle tC(B, C, corner1, randColor());
    scene_objects.push_back(dynamic_cast<Object *>(&t1));
    scene_objects.push_back(dynamic_cast<Object *>(&t2));
    scene_objects.push_back(dynamic_cast<Object *>(&t3));
    scene_objects.push_back(dynamic_cast<Object *>(&t4));
    scene_objects.push_back(dynamic_cast<Object *>(&t5));
    scene_objects.push_back(dynamic_cast<Object *>(&t6));
    scene_objects.push_back(dynamic_cast<Object *>(&t7));
    scene_objects.push_back(dynamic_cast<Object *>(&t8));
    scene_objects.push_back(dynamic_cast<Object *>(&t9));
    scene_objects.push_back(dynamic_cast<Object *>(&tA));
    scene_objects.push_back(dynamic_cast<Object *>(&tB));
    scene_objects.push_back(dynamic_cast<Object *>(&tC));
    scene_objects.clear();
    PLYReader::readFromPly(scene_objects, "sceneObjects.ply");
    /* // Add spheres of random color, size, and position
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
    BVH * bounding_volume_hierarchy = new BVH(scene_objects, Axis('x'));
    int aa_index;
    double xamnt, yamnt;
    for (size_t x = 0; x < width; ++x) {
        std::cout << "x = " << x << "\r" << std::flush;
        for (size_t y = 0; y < height; ++y) {
            // For every pixel...
            pixelindex = y * width + x;
            std::vector<double> tempRed(aadepth * aadepth, 0);
            std::vector<double> tempGreen(aadepth * aadepth, 0);
            std::vector<double> tempBlue(aadepth * aadepth, 0);
            // Shoot additional rays for antialiasing per pixel
            for (int aax = 0; aax < aadepth; ++aax) {
                for (int aay = 0; aay < aadepth; ++aay) {
                    ++raysFired;
                    aa_index = aay * aadepth + aax;
                    if (aadepth == 1) {
                        // No anti-aliasing, make normal perspective illusion
                        if (width > height) {
                            xamnt = ((x + 0.5) / width) * aspectRatio -
                                    (((width - height) / (double) height) / 2);
                            yamnt = ((height - y) + 0.5) / height;
                        } else if (height > width) {
                            xamnt = (x + 0.5) / width;
                            yamnt = (((height - y) + 0.5) / height) /
                                    aspectRatio -
                                    (((height - width) / (double) width) / 2);
                        } else {
                            xamnt = (x + 0.5) / width;
                            yamnt = ((height - y) + 0.5) / height;
                        }
                    } else {
                        // Replace 0.5 above with aax / aadepth - 1
                        if (width > height) {
                            xamnt = ((x + (double) aax / ((double) aadepth - 1))
                                    / width) * aspectRatio -
                                    (((width - height) / (double) height) / 2);
                            yamnt = ((height - y) + (double) aax /
                                     ((double) aadepth - 1)) / height;
                        } else if (height > width) {
                            xamnt = (x + (double) aax / ((double) aadepth - 1))
                                    / width;
                            yamnt = (((height - y) + (double) aax /
                                     ((double) aadepth - 1)) / height)
                                    / aspectRatio - (((height - width) /
                                                      (double) width) / 2);
                        } else {
                            xamnt = (x + (double) aax / ((double) aadepth - 1))
                                    / width;
                            yamnt = ((height - y) + (double) aax /
                                     ((double) aadepth - 1)) / height;
                        }
                    }
                    Vector3D cam_ray_origin = scene_cam.getPosition();
                    Vector3D cam_ray_direction = (camdir + (camright * (xamnt - 0.5)) +
                                                 (camdown * (yamnt - 0.5))).normalize();
                    Ray3D cam_ray(cam_ray_origin, cam_ray_direction);
                    // Loop through objs and see if the ray intersects any objs
                    /* *** Without BVH
                    Intersection intersection = getIntersection(scene_objects,
                                                                cam_ray);
                    */
                    Intersection intersection = bounding_volume_hierarchy->findIntersection(cam_ray);
                    Object * foremostObjPtr = intersection.objPtr;
                    double minDistance = intersection.distance;
                    // Index of the least positive intersection is the closest object
                    if (foremostObjPtr == nullptr) {
                        // Set the background to black
                        tempRed[aa_index] = 0;
                        tempGreen[aa_index] = 0;
                        tempBlue[aa_index] = 0;
                    } else {
                        if (minDistance > accuracy) {
                            Vector3D intersectionPoint = cam_ray_origin +
                                (cam_ray_direction * minDistance);
                            Vector3D intersectionRayDirection = cam_ray_direction;
                            // Resolve shadow
                            Color c = getColorAt(intersectionPoint,
                                                 intersectionRayDirection,
                                                 scene_objects,
                                                 bounding_volume_hierarchy,
                                                 foremostObjPtr,
                                                 scene_lights,
                                                 accuracy,
                                                 ambientLight,
                                                 raysFired,
                                                 objsQueried);
                            // Get color intersection
                            tempRed[aa_index] = c.getRed();
                            tempGreen[aa_index] = c.getGreen();
                            tempBlue[aa_index] = c.getBlue();
                        }
                    }
                }
            }
            double totalRed = 0;
            double totalGreen = 0;
            double totalBlue = 0;
            for (int i = 0; i < aadepth * aadepth; ++i) {
                totalRed += tempRed[i];
                totalGreen += tempGreen[i];
                totalBlue += tempBlue[i];
            }
            double avgRed = totalRed / (aadepth * aadepth);
            double avgGreen = totalGreen / (aadepth * aadepth);
            double avgBlue = totalBlue / (aadepth * aadepth);
            // Use average color of all rays shot at a pixel to antialias
            pixels[pixelindex].r = avgRed;
            pixels[pixelindex].g = avgGreen;
            pixels[pixelindex].b = avgBlue;
        }
    }
    PPMWriter::saveppm("scene.ppm", width, height, pixels);
    end = clock();
    float elapsed = ((float) end - (float) start) / CLOCKS_PER_SEC;
    std::cout << elapsed << " seconds.\n";
    std::cout << raysFired << " rays fired.\n";
    std::cout << objsQueried << " objs queried.\n";
    delete bounding_volume_hierarchy;
    for (auto objPtr : scene_objects) {
        free(objPtr);
    }
    return 0;
}
