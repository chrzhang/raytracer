#ifndef __GRID__
#define __GRID__

#include "Ray3D.hpp"

struct Grid {
    void findCellsIntersectedBy(const Ray3D &);
};

#endif
