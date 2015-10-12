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

#include "RGBType.h"
#include "PPMWriter.h"
#include "Vector3D.h"
#include "Ray3D.h"
#include "Camera.h"

int main() {

    std::cout << "Rendering..." << std::endl;

    srand(time(NULL));

    size_t width = 640;
    size_t height = 480;
    size_t n = width * height;

    RGBType * pixels = new RGBType[n];

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

    for (size_t x = 0; x < width; ++x) {

        for (size_t y = 0; y < height; ++y) {
            pixelindex = y * width + x;
            // Return color

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
