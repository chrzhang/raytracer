#include "Triangle.hpp"
#include <algorithm>

Triangle::Triangle(const Vector3D & epA, const Vector3D & epB,
                   const Vector3D & epC, Color color)
                    : epA(epA), epB(epB), epC(epC), color(color) {
    normal = (epC - epA).crossProduct(epB - epA).normalize();
    distance = normal.dotProduct(epA);
    setBBox(std::min({epA.getX(), epB.getX(), epC.getX()}),
            std::min({epA.getY(), epB.getY(), epC.getY()}),
            std::min({epA.getZ(), epB.getZ(), epC.getZ()}),
            std::max({epA.getX(), epB.getX(), epC.getX()}),
            std::max({epA.getY(), epB.getY(), epC.getY()}),
            std::max({epA.getZ(), epB.getZ(), epC.getZ()}));
}

Vector3D Triangle::getNormal() const { return normal; }

Vector3D Triangle::getCentroid() const {
    return Vector3D((epA.getX() + epB.getX() + epC.getX()) / 3,
                    (epA.getY() + epB.getY() + epC.getY()) / 3,
                    (epA.getZ() + epB.getZ() + epC.getZ()) / 3);
}

Color Triangle::getColor() const { return color; }

double Triangle::getDistance() const { return distance; }

Vector3D Triangle::getNormalAt(const Vector3D &) const {
    return normal.invert();
}

void Triangle::setBBox(double x_min, double y_min, double z_min,
                       double x_max, double y_max, double z_max) {
    Object::setBBox(x_min, y_min, z_min, x_max, y_max, z_max);
}

BoundingBox Triangle::getBBox() const {
    return Object::getBBox();
}

// Return distance from ray origin to intersection
// See comments for variables and the equations in Plane.cpp's findIntersection
double Triangle::findIntersection(const Ray3D & ray) const {
    // See if the ray intersects the bounding box
    // *** With bounding box
    if (!Object::intersectsBBox(ray)) { return -1; }
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
