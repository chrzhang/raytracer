#ifndef __RAY3D__
#define __RAY3D__

#include "Vector3D.hpp"

class Ray3D {
    Vector3D origin, direction;
    public:
        Ray3D();
        Ray3D(Vector3D origin, Vector3D direction);
        Vector3D getOrigin() const;
        Vector3D getDirection() const;
};

#endif
