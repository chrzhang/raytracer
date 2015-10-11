#ifndef __PPMWRITER__
#define __PPMWRITER__

#include <cstdlib>

#include "RGBType.h"

class PPMWriter {

    public:

        static void saveppm(const char * filename, size_t width, size_t height,
                            RGBType * data);

};

#endif
