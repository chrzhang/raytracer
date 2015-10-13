#include "Light.hpp"
#include "Vector3D.hpp"
#include "Color.hpp"

Light::Light() {

    position = Vector3D(0,0,0);
    color = Color(1,1,1,0);

}

Light::Light(Vector3D position, Color color) {

    this->position = position;
    this->color = color;

}

Vector3D Light::getPosition() const {

    return position;

}

Color Light::getColor() const {

    return color;

}
