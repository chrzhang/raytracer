#ifndef __VECTOR3D__
#define __VECTOR3D__

#include <iostream>

class Vector3D {
    double x, y, z;
    public:
        Vector3D();
        Vector3D(double, double, double);
        double getX(void) const;
        double getY(void) const;
        double getZ(void) const;
        void setX(double);
        void setY(double);
        void setZ(double);
        double getMagnitude(void) const;
        Vector3D normalize(void) const;
        Vector3D invert(void) const;
        double dotProduct(const Vector3D &) const;
        Vector3D crossProduct(const Vector3D &) const;
        friend Vector3D operator+(const Vector3D &, const Vector3D &);
        friend Vector3D operator-(const Vector3D &, const Vector3D &);
        friend Vector3D operator*(const Vector3D &, double);
        friend Vector3D operator*(double, const Vector3D &);
        friend Vector3D operator/(double, const Vector3D &);
        friend Vector3D operator/(const Vector3D &, double);
        friend std::ostream & operator<<(std::ostream &, const Vector3D &);
        friend bool operator<(const Vector3D &, const Vector3D &);
        friend bool operator>(const Vector3D &, const Vector3D &);
};

#endif
