#ifndef __BOX__
#define __BOX__

#include "Object.hpp"
#include "Vector3D.hpp"
#include "Ray3D.hpp"
#include "Color.hpp"

class Box : public Object {
    Vector3D min, max;
    Color color;
    public:
        Box(const Vector3D & min, const Vector3D & max, Color color);
        virtual Color getColor() const;
        virtual Vector3D getNormalAt(Vector3D point) const;
        virtual double findIntersection(Ray3D ray) const;
};

#endif
