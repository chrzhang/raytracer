#ifndef __GRID__
#define __GRID__

#include "Ray3D.hpp"
#include "Object.hpp"
#include "Intersection.hpp"
#include "BoundingBox.hpp"

#include <vector>

class Grid {
    BoundingBox bbox;
    double x_len, y_len, z_len;
    Vector3D gridDim, gridRes, gridMin, gridMax;
    std::vector<std::vector<std::vector<std::vector<Object *>>>> matrix;
    int getSign(double) const;
    bool outOfBounds(const Vector3D &) const;
    public:
        Grid(const Vector3D &, const Vector3D &, const Vector3D &);
        Grid(const std::vector<Object *> &);
        void findCellsIntersectedBy(const Ray3D &);
        Intersection findIntersection(const Ray3D &, bool) const;
        void printIndicesContaining(Object *) const;
};

#endif
