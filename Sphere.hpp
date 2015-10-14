#ifndef __SPHERE__
#define __SPHERE__

#include "Vector3D.hpp"
#include "Color.hpp"
#include "Object.hpp"
#include "Ray3D.hpp"

#include <cmath>


class Sphere : public Object {

    Vector3D center;
    double radius;
    Color color;

    public:

        Sphere();
        Sphere(Vector3D center, double radius, Color color);

        Vector3D getCenter() const;
        double getRadius() const;
        virtual Color getColor() const;

        virtual Vector3D getNormalAt(Vector3D point) const;
        virtual double findIntersection(Ray3D ray) const;

};

#endif
