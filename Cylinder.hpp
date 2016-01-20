#ifndef __CYLINDER__
#define __CYLINDER__

#include "Vector3D.hpp"
#include "Color.hpp"
#include "Object.hpp"
#include "Ray3D.hpp"
#include <cmath>

class Cylinder : public Object { // TODO use opt args for closed cylinder
    double radius;
    Color color;
    public:
        Cylinder(double radius, Color color);
        double getRadius() const;
        virtual Color getColor() const;
        virtual Vector3D getNormalAt(Vector3D point) const;
        virtual double findIntersection(Ray3D ray) const;
};

#endif
