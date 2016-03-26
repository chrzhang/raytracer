#include "Ray3D.hpp"

Ray3D::Ray3D(Vector3D o, Vector3D d) : origin(o), direction(d),
                                       invDirection(1 / d) {}

Vector3D Ray3D::getOrigin() const { return origin; }

Vector3D Ray3D::getDirection() const { return direction; }

Vector3D Ray3D::getInvDirection() const { return invDirection; }
