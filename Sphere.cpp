#include "Sphere.hpp"
#include <assert.h>

Sphere::Sphere(Vector3D cen, double r, Color col)
    : center(cen), radius(r), color(col) {}

Vector3D Sphere::getCenter() const { return center; }

double Sphere::getRadius() const { return radius; }

Color Sphere::getColor() const { return color; }

Vector3D Sphere::getNormalAt(Vector3D point) const {
    // Normal points away from origin of sphere
    Vector3D normal = (point - center).normalize();
    return normal;
}

double Sphere::findIntersection(Ray3D ray) const {
    // To find intersection between sphere and arbitrary ray, substitute the ray
    // equation into the sphere equation
    // Ray: x(t) = x_origin + tx_d (where x_d is the ray offset)
    // Sphere: x^2 + y^2 + z^2 = r^2
    // So substituting:
    // (x_origin + tx_d)^2 + (y_origin + ty_d)^2 + (z_origin + tz_d)^2 = r^2
    // We get: (Note the quadratic formula nature)
    // t^2(x_d^2 + y_d^2 + z_d^2) +
    // t(2x_origin*x_d + 2y_origin*y_d + 2z_origin*z_d) +
    // (x_origin^2 + y_origin^2 + z_origin^2 - r^2) = 0
    Vector3D rayOrigin = ray.getOrigin();
    Vector3D rayDirection = ray.getDirection();
    double a = 1; // Because our Ray3D's are normalized
    double b = (2 * (rayOrigin.getX() - center.getX()) * rayDirection.getX()) +
               (2 * (rayOrigin.getY() - center.getY()) * rayDirection.getY()) +
               (2 * (rayOrigin.getZ() - center.getZ()) * rayDirection.getZ());
    double c = pow(rayOrigin.getX() - center.getX(), 2) +
               pow(rayOrigin.getY() - center.getY(), 2) +
               pow(rayOrigin.getZ() - center.getZ(), 2) -
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
