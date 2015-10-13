#ifndef __LIGHT__
#define __LIGHT__

#include "Vector3D.hpp"
#include "Color.hpp"

class Light {

    Vector3D position;
    Color color;

    public:

        Light();
        Light(Vector3D position, Color color);
        Vector3D getPosition() const;
        Color getColor() const;

};

#endif
