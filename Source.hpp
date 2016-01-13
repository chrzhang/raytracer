#ifndef __SOURCE__
#define __SOURCE__

#include "Ray3D.hpp"
#include "Vector3D.hpp"
#include "Color.hpp"

class Source {
    public:
        Source();
        virtual Vector3D getPosition() const = 0;
        virtual Color getColor() const = 0;
};

#endif
