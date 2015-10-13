#ifndef __OBJECT__
#define __OBJECT__

#include "Ray3D.hpp"
#include "Vector3D.hpp"
#include "Color.hpp"

class Object {

    public:

        Object();

        Color getColor() const;

        virtual double findIntersection(Ray3D ray) const;

};

#endif
