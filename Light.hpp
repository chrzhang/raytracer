#ifndef __LIGHT__
#define __LIGHT__

#include "Source.hpp"
#include "Vector3D.hpp"
#include "Color.hpp"

class Light : public Source {

    Vector3D position;
    Color color;

    public:

        Light();
        Light(Vector3D position, Color color);
        virtual Vector3D getPosition() const;
        virtual Color getColor() const;

};

#endif
