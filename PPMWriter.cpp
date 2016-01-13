#include "PPMWriter.hpp"
#include <fstream>
#include <iostream>
#include <ios>
#include <cmath>
#include <vector>
#include "assert.h"

void PPMWriter::saveppm(const char * filename, size_t width, size_t height,
                        std::vector<RGBType> data) {
    assert(width != 0);
    assert(height != 0);
    std::ofstream ofs;
    try {
        ofs.open(filename, std::ios::binary);
        if (ofs.fail()) {
            throw("Can't open output file.");
        }
        ofs << "P6\n" << width << " " << height << "\n255\n";
        unsigned char r, g, b;
        for (size_t i = 0; i < width * height; ++i) {
            //r = (char) floor(data[i].r * 255);
            r = static_cast<unsigned char>(floor(data[i].r * 255));
            //g = (char) floor(data[i].g * 255);
            g = static_cast<unsigned char>(floor(data[i].g * 255));
            //b = (char) floor(data[i].b * 255);
            b = static_cast<unsigned char>(floor(data[i].b * 255));
            ofs << r << g << b;
        }
        ofs.close();
    } catch (const char * err) {
        fprintf(stderr, "%s\n", err);
        ofs.close();
    }
}
