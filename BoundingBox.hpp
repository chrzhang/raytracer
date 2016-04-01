#ifndef __BOUNDINGBOX__
#define __BOUNDINGBOX__

#include <iostream>

class Ray3D;

struct BoundingBox {
    double x_min, y_min, z_min, x_max, y_max, z_max;
    BoundingBox();
    void set(double, double, double, double, double, double);
    bool intersects(const Ray3D &) const;
    friend std::ostream & operator<<(std::ostream & os,
                                     const BoundingBox & bbox);
};

#endif
