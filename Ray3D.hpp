#ifndef __RAY3D__
#define __RAY3D__

#include "Vector3D.hpp"

class Ray3D {
    Vector3D origin, direction, invDirection;
    public:
        Ray3D(Vector3D origin, Vector3D direction);
        Vector3D getOrigin() const;
        Vector3D getDirection() const;
        Vector3D getInvDirection() const;
};

#endif
