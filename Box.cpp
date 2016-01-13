#include "Box.hpp"
#include "assert.h"

Box::Box() {
    min = Vector3D(0, 0, 0);
    max = Vector3D(1, 1, 1);
    color = Color(0.5, 0.5, 0.5, 0);
}

Box::Box(const Vector3D & min, const Vector3D & max, Color color)
    : min(min), max(max), color(color) {}

Color Box::getColor() const {
    return color;
}

Vector3D Box::getNormalAt(Vector3D point) const {
    // TODO Check if the point is on the cube
    // Determine which plane is defined by the point
    // Use our knowledge about planes to get the normal
    assert(0);
    return Vector3D(0,0,0);

}

double Box::findIntersection(Ray3D ray) const {
    Vector3D rayDirection = ray.getDirection();
    Vector3D rayOrigin = ray.getOrigin();
    double tmin = (min.getX() - rayOrigin.getX()) / rayDirection.getX();
    double tmax = (max.getX() - rayOrigin.getX()) / rayDirection.getX();
    if (tmin > tmax) {
        double temp = tmin;
        tmin = tmax;
        tmax = temp;
    }
    double tymin = (min.getY() - rayOrigin.getY()) / rayDirection.getY();
    double tymax = (max.getY() - rayOrigin.getY()) / rayDirection.getY();
    if (tymin > tymax) {
        double temp = tymin;
        tymin = tymax;
        tymax = temp;
    }
    /*
    A box is defined as the intersection of three pairs of parallel planes.
    Projected onto 2D space, the ray will miss a box if it crosses through
    one set of planes completely before starting another set of planes.
    */
    if ((tmin > tymax) || (tymin > tmax)) {
        // Missed
        return -1;
    }
    if (tymin > tmin)
        tmin = tymin;
    if (tymax > tmax)
        tmax = tymax;
    double tzmin = (min.getZ() - rayOrigin.getZ()) / rayDirection.getZ();
    double tzmax = (max.getZ() - rayOrigin.getZ()) / rayDirection.getZ();
    if (tzmin > tzmax) {
        double temp = tzmin;
        tzmin = tzmax;
        tzmax = temp;
    }
    if ((tmin > tzmax) || (tzmin > tmax)) {
        return -1;
    }
    if (tzmin > tmin) { tmin = tzmin; }
    if (tzmax < tmax) { tmax = tzmax; }
    // It hits, so now find the distance
    return tmin;
}
