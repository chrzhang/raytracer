#ifndef __PPMWRITER__
#define __PPMWRITER__

#include <cstdlib>
#include <vector>

#include "RGBType.hpp"

class PPMWriter {

    public:

        static void saveppm(const char * filename, size_t width, size_t height,
                            std::vector<RGBType> data);

};

#endif
