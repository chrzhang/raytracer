#include "Vector3D.hpp"
#include <cmath>
#include <iostream>

Vector3D::Vector3D() {
    x = y = z = 0;
}

Vector3D::Vector3D(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

double Vector3D::getX() const { return x; }

double Vector3D::getY() const { return y; }

double Vector3D::getZ() const { return z; }

double Vector3D::getMagnitude() const {
    return sqrt((x * x) + (y * y) + (z * z));
}

Vector3D Vector3D::normalize() const {
    double mag = getMagnitude();
    return Vector3D(x / mag, y / mag, z / mag);
}

Vector3D Vector3D::invert() const {
    return Vector3D(-x, -y, -z);
}

double Vector3D::dotProduct(const Vector3D & v) const {
    return x * v.getX() + y * v.getY() + z * v.getZ();
}

Vector3D Vector3D::crossProduct(const Vector3D & v) const {
    return Vector3D(y * v.getZ() - z * v.getY(),
                    z * v.getX() - x * v.getZ(),
                    x * v.getY() - y * v.getX());
}

Vector3D operator+(const Vector3D & lhs, const Vector3D & rhs) {
    return Vector3D(lhs.getX() + rhs.getX(),
                    lhs.getY() + rhs.getY(),
                    lhs.getZ() + rhs.getZ());
}

Vector3D operator-(const Vector3D & lhs, const Vector3D & rhs) {
    return Vector3D(lhs.getX() - rhs.getX(),
                    lhs.getY() - rhs.getY(),
                    lhs.getZ() - rhs.getZ());
}

Vector3D operator*(const Vector3D & v, double scale) {
    return Vector3D(scale * v.getX(),
                    scale * v.getY(),
                    scale * v.getZ());
}

Vector3D operator*(double scale, const Vector3D & v) {
    return v * scale;
}

std::ostream & operator<<(std::ostream & os, const Vector3D & v) {
    os << "(" << v.getX() << "," << v.getY() << "," << v.getZ() << ")";
    return os;
}
