#include "Plane.hpp"

Plane::Plane() {

    normal = Vector3D(1, 0, 0);
    distance = 0.0;
    color = Color(0.5, 0.5, 0.5, 0);

}

Plane::Plane(Vector3D normal, double distance, Color color) {

    this->normal = normal;
    this->distance = distance;
    this->color = color;

}

Vector3D Plane::getNormal() const {

    return normal;

}

double Plane::getDistance() const {

    return distance;

}

Color Plane::getColor() const {

    return color;

}

Vector3D Plane::getNormalAt(Vector3D point) const {

    (void) point;

    return normal;

}

// Return distance from ray origin to intersection
/**
 * A ray is represented by l0 + l * t = p
 *      l0 - ray origin
 *      l  - ray direction
 *      t  - parameter
 *      p  - point on plane
 * A plane is represented by (p - p0) dot n = 0 (because perpendicular)
 *      p0 - a point representing the distance from the origin
 *      n  - normal of plane
 * Solving for t by substituting p gives that
 * t = [ (p0 - l0) dot n ] / [ l dot n ]
 */
double Plane::findIntersection(Ray3D ray) const {

    Vector3D rayDirection = ray.getDirection();

    Vector3D l = rayDirection;
    Vector3D n = normal;

    double ldotn = l.dotProduct(n);

    if (0 == ldotn) { // Ray is || to plane

        return -1; // Never intersects

    } else {

        Vector3D p0 = normal * distance;
        Vector3D l0 = ray.getOrigin();

        double numerator = (p0 - l0).dotProduct(n);

        return numerator / ldotn;

    }

}
