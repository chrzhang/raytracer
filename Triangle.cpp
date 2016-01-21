#include "Triangle.hpp"

Triangle::Triangle(const Vector3D & epA, const Vector3D & epB,
                   const Vector3D & epC, Color color)
                    : epA(epA), epB(epB), epC(epC), color(color) {
    normal = (epC - epA).crossProduct(epB - epA).normalize();
    distance = normal.dotProduct(epA);
}

Vector3D Triangle::getNormal() const { return normal; }

Color Triangle::getColor() const { return color; }

double Triangle::getDistance() const { return distance; }

Vector3D Triangle::getNormalAt(Vector3D) const { return normal.invert(); }

// Return distance from ray origin to intersection
// See comments for variables and the equations in Plane.cpp's findIntersection
double Triangle::findIntersection(Ray3D ray) const {
    // First check if the ray intersects with the plane (use same calculations)
    Vector3D rayDirection = ray.getDirection();
    Vector3D rayOrigin = ray.getOrigin();
    double ldotn = rayDirection.dotProduct(normal);
    if (0 == ldotn) { // Ray is || to triangle
        return -1;
    } else {
        Vector3D p0 = normal * distance;
        double distanceToPlane = (p0 - rayOrigin).dotProduct(normal) / ldotn;
        // Then see if the point is inside the triangle (3 conditions)
        // Q is the point of intersection
        Vector3D Q = (rayDirection * distanceToPlane) + rayOrigin;
        Vector3D sideCA = epC - epA;
        Vector3D segQA = Q - epA;
        // 1. (CA x QA) * n >= 0
        if (sideCA.crossProduct(segQA).dotProduct(normal) < 0) {
            return -1;
        }
        Vector3D sideBC = epB - epC;
        Vector3D segQC = Q - epC;
        // 2. (BC x QC) * n >= 0
        if (sideBC.crossProduct(segQC).dotProduct(normal) < 0) {
            return -1;
        }
        Vector3D sideAB = epA - epB;
        Vector3D segQB = Q - epB;
        // 3. (AB x QB) * n >= 0
        if (sideAB.crossProduct(segQB).dotProduct(normal) < 0) {
            return -1;
        }
        return distanceToPlane;
    }
}
