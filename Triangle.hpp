#ifndef __TRIANGLE__
#define __TRIANGLE__

#include "Object.hpp"
#include "Vector3D.hpp"
#include "Ray3D.hpp"
#include "Color.hpp"

class Triangle : public Object {
    Vector3D epA, epB, epC; // Endpoints of the triangle
    Vector3D normal;
    double distance;
    Color color;
    public:
        Triangle(const Vector3D & epA,
                 const Vector3D & epB,
                 const Vector3D & epC,
                 Color color);
        Vector3D getNormal() const;
        double getDistance() const;
        virtual Color getColor() const;
        virtual Vector3D getNormalAt(Vector3D point) const;
        virtual double findIntersection(Ray3D ray) const;
};

#endif
