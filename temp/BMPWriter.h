#ifndef __BMPWRITER__
#define __BMPWRITER__

#include "RGBType.h"

class BMPWriter {

    public:

        static void savebmp(const char* filename, int width, int height,
                            int dpi, RGBType* data);

};

#endif
