#ifndef __PLANE__
#define __PLANE__

#include "Vector3D.hpp"
#include "Color.hpp"
#include "Object.hpp"
#include "Ray3D.hpp"

#include <cmath>

class Plane : public Object {
    Vector3D normal;
    double distance;
    Color color;
    public:
        Plane(Vector3D normal, double distance, Color color);
        Vector3D getNormal() const;
        double getDistance() const;
        virtual Color getColor() const;
        virtual Vector3D getNormalAt(const Vector3D & point) const;
        virtual double findIntersection(const Ray3D & ray) const;
};

#endif
