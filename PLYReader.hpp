#ifndef __PLYREADER__
#define __PLYREADER__

#include "Object.hpp"
#include <vector>
#include <string>

class PLYReader {
    public:
        static void readFromPly(
            std::vector<Object *> & scene_objects,
            const std::string & filename);
};

#endif
