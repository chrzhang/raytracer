#ifndef __CAMERA__
#define __CAMERA__

#include "Vector3D.hpp"

class Camera {
    Vector3D position;
    Vector3D direction;
    // For coordinate system
    Vector3D right;
    Vector3D down;
    public:
        Camera(Vector3D position, Vector3D direction, Vector3D right,
               Vector3D down);
        Vector3D getPosition() const;
        Vector3D getDirection() const;
        Vector3D getRight() const;
        Vector3D getDown() const;
};

#endif
