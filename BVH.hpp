#ifndef __BVH__
#define __BVH__

#include "BoundingBox.hpp"
#include "Object.hpp"
#include "Axis.hpp"
#include <vector>

class BVH {
    BoundingBox bbox;
    BVH * left;
    BVH * right;
    std::vector<Object *> objsIncluded;
    public:
        BVH(const std::vector<Object *> & sceneObjs, const Axis & axis);
        void print() const;
        ~BVH();
};

#endif
