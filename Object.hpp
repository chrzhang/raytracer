#ifndef __OBJECT__
#define __OBJECT__

#include "Ray3D.hpp"
#include "Vector3D.hpp"
#include "Color.hpp"
#include "BoundingBox.hpp"

class Object {
    BoundingBox bbox;
    public:
        Object();
        virtual Color getColor() const = 0;
        virtual Vector3D getNormalAt(const Vector3D &) const = 0;
        virtual double findIntersection(const Ray3D &) const = 0;
        BoundingBox getBBox() const;
        void setBBox(double, double, double, double, double, double);
        double intersectsBBox(const Ray3D &) const;
};

#endif
