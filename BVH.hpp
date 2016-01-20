#ifndef __BVH__
#define __BVH__

#include "BVHNode.hpp"
#include "Object.hpp"
#include <vector>

class BVH {
    BVHNode * root;
    public:
        BVH();
        void construct(const std::vector<Object *> & primitives);
};

#endif
