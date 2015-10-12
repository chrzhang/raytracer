#include "Vector3D.h"

Vector3D::Vector3D() {

    x = y = z = 0;

}

Vector3D::Vector3D(double x, double y, double z) {

    this->x = x;
    this->y = y;
    this->z = z;

}

double Vector3D::getX() const {

    return x;

}

double Vector3D::getY() const {

    return y;

}

double Vector3D::getZ() const {

    return z;

}
