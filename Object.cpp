#include "Object.hpp"

Object::Object() {
}

Color Object::getColor() const {

    return Color(0.0, 0.0, 0.0, 0);

}

double Object::findIntersection(Ray3D ray) const {

    (void) ray;

    return 0.0;

}
