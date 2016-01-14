#include "Cylinder.hpp"

Cylinder::Cylinder() {
    radius = 1.0;
    color = Color(0.5, 0.5, 0.5, 0);
}

Cylinder::Cylinder(double r, Color col) : radius(r), color(col) {}

double Cylinder::getRadius() const { return radius; }

Color Cylinder::getColor() const { return color; }

Vector3D Cylinder::getNormalAt(Vector3D point) const {
    // When along an axis, that axis has values, everything else is 0
    Vector3D normal = (point - Vector3D(0, 0, point.getZ())).normalize();
    return normal;
}

double Cylinder::findIntersection(Ray3D ray) const {
    Vector3D rayOrigin = ray.getOrigin();
    Vector3D rayDirection = ray.getDirection();
    double a = pow(rayDirection.getX(), 2) +
               pow(rayDirection.getY(), 2);
    double b = 2 * (rayOrigin.getX() * rayDirection.getX()) +
               2 * (rayOrigin.getY() * rayDirection.getY());
    double c = pow(rayOrigin.getX(), 2) + pow(rayOrigin.getY(), 2) -
               pow(radius, 2);
    double discriminant = b * b - 4 * a * c;
    if (discriminant > 0) {
        // Ray intersects sphere but does it on both sides of the sphere
        double root1 = ((-1 * b - sqrt(discriminant)) / (2 * a)) - 0.0000001;
        if (root1 > 0) {
            return root1; // smallest positive root
        } else {
            double root2 = ((sqrt(discriminant) - b) / (2 * a)) - 0.0000001;
            return root2;
        }
    } else {
        return -1;
    }
}
