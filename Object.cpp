#include "Object.hpp"

Object::Object() {}

BoundingBox Object::getBBox() const {
    return bbox;
}

void Object::setBBox(double x_min, double y_min, double z_min,
                     double x_max, double y_max, double z_max) {
    bbox.set(x_min, y_min, z_min, x_max, y_max, z_max);
}

bool Object::intersectsBBox(const Ray3D & ray) const {
    return true;
}
