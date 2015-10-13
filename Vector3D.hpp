#ifndef __VECTOR3D__
#define __VECTOR3D__

#include <iostream>

class Vector3D {

    double x, y, z;

    public:

        Vector3D();
        Vector3D(double x, double y, double z);
        double getX() const;
        double getY() const;
        double getZ() const;
        double getMagnitude() const;
        Vector3D normalize() const;
        Vector3D invert() const;
        double dotProduct(const Vector3D & v) const;
        Vector3D crossProduct(const Vector3D & v) const;
        friend Vector3D operator+(const Vector3D & lhs, const Vector3D & rhs);
        friend Vector3D operator-(const Vector3D & lhs, const Vector3D & rhs);
        friend Vector3D operator*(const Vector3D & v, double scale);
        friend Vector3D operator*(double scale, const Vector3D & v);
        friend std::ostream & operator<<(std::ostream & os, const Vector3D & v);

};

#endif
