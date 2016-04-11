#include "Intersection.hpp"

bool operator==(const Intersection & i1, const Intersection & i2) {
    return i1.objPtr == i2.objPtr && i1.distance == i2.distance;
}

bool operator!=(const Intersection & i1, const Intersection & i2) {
    return !(i1 == i2);
}
