#include "Camera.hpp"

Camera::Camera() {
    position = Vector3D(0,0,0);
    direction = Vector3D(0,0,1);
    right = Vector3D(0,0,0);
    down = Vector3D(0,0,0);
}

Camera::Camera(Vector3D pos, Vector3D dir, Vector3D right, Vector3D down) :
    position(pos), direction(dir), right(right), down(down) {}

Vector3D Camera::getPosition() const { return position; }

Vector3D Camera::getDirection() const { return direction; }

Vector3D Camera::getRight() const { return right; }

Vector3D Camera::getDown() const { return down; }
