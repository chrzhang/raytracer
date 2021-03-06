#include "BMPWriter.h"
#include <cmath>
#include <fstream>

void BMPWriter::savebmp(const char* filename, int width, int height, int dpi,
                        RGBType* data) {
/*
    FILE *f;
    int k = width * height;
    int s = 4 * k;
    int filesize = 54 + s;

    int m = static_cast<int>(39.375);
    int ppm = dpi * m; // pixels per meter

    unsigned char bmpfileheader[14] = {
        'B', 'M', 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0
    };

    unsigned char bmpinfoheader[40] = {
        40, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 1, 0, 24, 0
    };


    bmpfileheader[2] = (unsigned char)(filesize);
    bmpfileheader[3] = (unsigned char)(filesize >> 8);
    bmpfileheader[4] = (unsigned char)(filesize >> 16);
    bmpfileheader[5] = (unsigned char)(filesize >> 24);

    bmpinfoheader[4] = (unsigned char)(width);
    bmpinfoheader[5] = (unsigned char)(width >> 8);
    bmpinfoheader[6] = (unsigned char)(width >> 16);
    bmpinfoheader[7] = (unsigned char)(width >> 24);

    bmpinfoheader[8] = (unsigned char)(height);
    bmpinfoheader[9] = (unsigned char)(height >> 8);
    bmpinfoheader[10] = (unsigned char)(height >> 16);
    bmpinfoheader[11] = (unsigned char)(height >> 24);

    bmpinfoheader[21] = (unsigned char)(s);
    bmpinfoheader[22] = (unsigned char)(s >> 8);
    bmpinfoheader[23] = (unsigned char)(s >> 16);
    bmpinfoheader[24] = (unsigned char)(s >> 24);

    bmpinfoheader[25] = (unsigned char)(ppm);
    bmpinfoheader[26] = (unsigned char)(ppm >> 8);
    bmpinfoheader[27] = (unsigned char)(ppm >> 16);
    bmpinfoheader[28] = (unsigned char)(ppm >> 24);

    bmpinfoheader[29] = (unsigned char)(ppm);
    bmpinfoheader[30] = (unsigned char)(ppm >> 8);
    bmpinfoheader[31] = (unsigned char)(ppm >> 16);
    bmpinfoheader[32] = (unsigned char)(ppm >> 24);

    f = fopen(filename, "wb");

    fwrite(bmpfileheader, 1, 14, f);
    fwrite(bmpinfoheader, 1, 40, f);

    for (int i = 0; i < k; i++) {
        RGBType rgb = data[i];

        double red = data[i].r * 255;
        double green = data[i].g * 255;
        double blue = data[i].b * 255;

        unsigned char color[3] = { (char) floor(blue),
                                   (char) floor(green),
                                   (char) floor(red)};

        fwrite(color, 1, 3, f);
    }

    fclose(f);

    */
}
