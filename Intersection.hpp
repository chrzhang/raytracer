#ifndef __INTERSECTION__
#define __INTERSECTION__

#include "Object.hpp"

struct Intersection {
    Object * objPtr;
    double distance;
    Intersection() : objPtr(nullptr), distance(-1) {}
    Intersection(Object * op, double d) : objPtr(op), distance(d) {}
};

#endif
