#ifndef __GRID__
#define __GRID__

#include "Ray3D.hpp"

class Grid {
    size_t x_len, y_len, z_len;
    public:
        Grid(size_t, size_t, size_t);
        void findCellsIntersectedBy(const Ray3D &);
};

#endif
