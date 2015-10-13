#include "Camera.hpp"

Camera::Camera() {

    position = Vector3D(0,0,0);
    direction = Vector3D(0,0,1);
    right = Vector3D(0,0,0);
    down = Vector3D(0,0,0);

}

Camera::Camera(Vector3D position, Vector3D direction, Vector3D right,
               Vector3D down) {

    this->position = position;
    this->direction = direction;
    this->right = right;
    this->down = down;

}

Vector3D Camera::getPosition() const {

    return position;

}

Vector3D Camera::getDirection() const {

    return direction;

}

Vector3D Camera::getRight() const {

    return right;

}

Vector3D Camera::getDown() const {

    return down;

}
