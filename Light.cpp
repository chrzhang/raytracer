#include "Light.hpp"
#include "Vector3D.hpp"
#include "Color.hpp"

Light::Light(Vector3D p, Color c) : position(p), color(c) {}

Vector3D Light::getPosition() const { return position; }

Color Light::getColor() const { return color; }
