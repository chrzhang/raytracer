#include "Sphere.hpp"

Sphere::Sphere() {

    center = Vector3D(0, 0, 0);
    radius = 1.0;
    color = Color(0.5, 0.5, 0.5, 0);

}

Sphere::Sphere(Vector3D center, double radius, Color color) {

    this->center = center;
    this->radius = radius;
    this->color = color;

}

Vector3D Sphere::getCenter() const {

    return center;

}

double Sphere::getRadius() const {

    return radius;

}

Color Sphere::getColor() const {

    return color;

}

Vector3D Sphere::getNormalAt(Vector3D point) const {

    // Normal points away from origin of sphere

    // TODO Assert point is on the surface of the sphere

    Vector3D normal = (point - center).normalize();

    return normal;


}

double Sphere::findIntersection(Ray3D ray) const {

    Vector3D rayOrigin = ray.getOrigin();
    double rayOriginX = rayOrigin.getX();
    double rayOriginY = rayOrigin.getY();
    double rayOriginZ = rayOrigin.getZ();

    Vector3D rayDirection = ray.getDirection();
    double rayDirectionX = rayDirection.getX();
    double rayDirectionY = rayDirection.getY();
    double rayDirectionZ = rayDirection.getZ();

    Vector3D sphereCenter = center;
    double sphereCenterX = sphereCenter.getX();
    double sphereCenterY = sphereCenter.getY();
    double sphereCenterZ = sphereCenter.getZ();

    double a = 1;
    double b = (2 * (rayOriginX - sphereCenterX) * rayDirectionX) +
               (2 * (rayOriginY - sphereCenterY) * rayDirectionY) +
               (2 * (rayOriginZ - sphereCenterZ) * rayDirectionZ);
    double c = pow(rayOriginX - sphereCenterX, 2) +
               pow(rayOriginY - sphereCenterY, 2) +
               pow(rayOriginZ - sphereCenterZ, 2) -
               (radius * radius);
    double discriminant = b * b - 4 * a * c;

    if (discriminant > 0) {
        // Ray intersects sphere but does it on both sides of the sphere
        double root1 = ((-1 * b - sqrt(discriminant)) / 2) - 0.0000001;
        if (root1 > 0) {
            return root1; // smallest positive root
        } else {
            double root2 = ((sqrt(discriminant) - b) / 2) - 0.0000001;
            return root2;
        }
    } else {
        // Ray missed sphere
        return -1;
    }

}
