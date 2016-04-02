#ifndef __BVH__
#define __BVH__

#include "BoundingBox.hpp"
#include "Object.hpp"
#include "Axis.hpp"
#include "Intersection.hpp"

#include <vector>

class BVH {
    BoundingBox bbox;
    BVH * left;
    BVH * right;
    std::vector<Object *> objsIncluded;
    public:
        BVH(const std::vector<Object *> &, const Axis &);
        void print() const;
        ~BVH();
        Intersection findIntersection(const Ray3D &) const;
};

#endif
