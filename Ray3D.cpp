#include "Ray3D.hpp"

Ray3D::Ray3D() {
    origin = Vector3D(0,0,0);
    direction = Vector3D(1,0,0);
}

Ray3D::Ray3D(Vector3D o, Vector3D d) : origin(o), direction(d) {}

Vector3D Ray3D::getOrigin() const { return origin; }

Vector3D Ray3D::getDirection() const { return direction; }
