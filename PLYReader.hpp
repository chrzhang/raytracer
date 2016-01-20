#ifndef __PLYREADER__
#define __PLYREADER__

#include "Object.hpp"
#include <vector>
#include <string>

class PLYReader {
    public:
        static std::vector<Object *> readFromPly(const std::string & filename);
};

#endif
