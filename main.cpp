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


int main() {

    std::cout << "Rendering..." << std::endl;

    srand(time(NULL));

    size_t width = 640;
    size_t height = 480;
    size_t n = width * height;

    RGBType * pixels = new RGBType[n];

    size_t pixelindex;
    for (size_t x = 0; x < width; ++x) {

        for (size_t y = 0; y < height; ++y) {
            pixelindex = y * width + x;
            // Return color
/*
            size_t lowerx = rand() % 200;
            size_t upperx = 200 + (rand() % 200);
            size_t lowery = rand() % 200;
            size_t uppery = 200 + (rand() % 80);
*/
           // if ((x > lowerx && x < upperx) && (y > lowery && y < uppery)) {
                pixels[pixelindex].r = rand() % 256;
                pixels[pixelindex].g = rand() % 256;
                pixels[pixelindex].b = rand() % 256;
          /*  } else {
                pixels[pixelindex].r = 0;
                pixels[pixelindex].g = 0;
                pixels[pixelindex].b = 0;
            } */

        }

    }

    PPMWriter::saveppm("scene.ppm", width, height, pixels);

    return 0;

}
