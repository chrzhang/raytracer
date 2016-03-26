#ifndef __TRIANGLE__
#define __TRIANGLE__

#include "Object.hpp"
#include "Vector3D.hpp"
#include "Ray3D.hpp"
#include "Color.hpp"
#include "BoundingBox.hpp"

class Triangle : public Object {
    Vector3D epA, epB, epC; // Endpoints of the triangle
    Vector3D normal;
    double distance;
    Color color;
    public:
        Triangle(const Vector3D &,
                 const Vector3D &,
                 const Vector3D &,
                 Color);
        Vector3D getNormal() const;
        double getDistance() const;
        virtual Color getColor() const;
        virtual Vector3D getNormalAt(const Vector3D &) const;
        virtual double findIntersection(const Ray3D &) const;
        virtual BoundingBox getBBox() const;
        virtual void setBBox(double, double, double, double, double, double);
};

#endif
