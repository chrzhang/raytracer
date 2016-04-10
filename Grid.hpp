#ifndef __GRID__
#define __GRID__

#include "Ray3D.hpp"

class Grid {
    size_t x_len, y_len, z_len;
    Vector3D gridDim, gridRes, gridMin, gridMax;
    int getSign(double);
    bool outOfBounds(const Vector3D &);
    public:
        Grid(const Vector3D &, const Vector3D &, const Vector3D &);
        void findCellsIntersectedBy(const Ray3D &);
};

#endif
