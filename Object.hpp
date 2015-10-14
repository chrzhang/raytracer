#ifndef __OBJECT__
#define __OBJECT__

#include "Ray3D.hpp"
#include "Vector3D.hpp"
#include "Color.hpp"

class Object {

    public:

        Object();

        virtual Color getColor() const = 0;

        virtual Vector3D getNormalAt(Vector3D point) const = 0;

        virtual double findIntersection(Ray3D ray) const = 0;

};

#endif
