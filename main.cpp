#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <cstdlib>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "RGBType.hpp"
#include "PPMWriter.hpp"
#include "Vector3D.hpp"
#include "Ray3D.hpp"
#include "Camera.hpp"
#include "Color.hpp"
#include "Light.hpp"
#include "Object.hpp"
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


int main() {

    std::cout << "Rendering..." << std::endl;
    srand(time(NULL));

    size_t width = 640;
    size_t height = 480;
    size_t n = width * height;
    double aspectRatio = (double) width / (double) height;

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
    Vector3D camdown = camright.crossProduct(camdir);
    std::cout << "camdown" << camdown<< std::endl;

    Camera scene_cam(campos, camdir, camright, camdown);

    Color white_light(1.0, 1.0, 1.0, 0);
    Color pretty_green(0.5, 1.0, 0.5, 0.3);
    Color gray(0.5, 0.5, 0.5, 0);
    Color black(0.0, 0.0, 0.0, 0);
    Color maroon(0.5, 0.25, 0.25, 0);

    Vector3D light_position(-7, 10, -10);
    Light scene_light(light_position, white_light);

    // Objects in scene
    Vector3D origin(0, 0, 0);
    Sphere s(origin, 1, pretty_green);
    Plane p(Y, -1, maroon);

    std::vector<Object *> scene_objects;
    scene_objects.push_back(dynamic_cast<Object *>(&s));
    scene_objects.push_back(dynamic_cast<Object *>(&p));

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
            std::cout << foremostObjIndex;

            size_t lowerx = 200;
            size_t upperx = 440;
            size_t lowery = 200;
            size_t uppery = 280;
            if ((x > lowerx && x < upperx) && (y > lowery && y < uppery)) {
                pixels[pixelindex].r = rand() % 256;
                pixels[pixelindex].g = rand() % 256;
                pixels[pixelindex].b = rand() % 256;
            } else {
                pixels[pixelindex].r = 0;
                pixels[pixelindex].g = 0;
                pixels[pixelindex].b = 0;
            }

        }

    }

    PPMWriter::saveppm("scene.ppm", width, height, pixels);

    return 0;

}
