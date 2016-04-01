#include "BoundingBox.hpp"
#include "Ray3D.hpp"
#include <cassert>

BoundingBox::BoundingBox() {
    x_min = y_min = z_min = x_max = y_max = z_max = 0;
}

void BoundingBox::set(double xn, double yn, double zn, double xx, double yx,
                      double zx) {
    assert(xn <= xx && yn <= yx && zn <= zx);
    x_min = xn;
    y_min = yn;
    z_min = zn;
    x_max = xx;
    y_max = yx;
    z_max = zx;
}

// Use axis-aligned bounding volume test for slabs to check for intersection
bool BoundingBox::intersects(const Ray3D & ray) const {
    double roX = ray.getOrigin().getX();
    double roY = ray.getOrigin().getY();
    double roZ = ray.getOrigin().getZ();
    double riX = ray.getInvDirection().getX();
    double riY = ray.getInvDirection().getY();
    double riZ = ray.getInvDirection().getZ();
    double tx_min = (x_min - roX) * riX;
    double tx_max = (x_max - roX) * riX;
    if (tx_min > tx_max) { std::swap(tx_min, tx_max); }
    double ty_min = (y_min - roY) * riY;
    double ty_max = (y_max - roY) * riY;
    if (ty_min > ty_max) { std::swap(ty_min, ty_max); }
    if (tx_min > ty_max || ty_min > tx_max) { return false; }
    double tz_min = (z_min - roZ) * riZ;
    double tz_max = (z_max - roZ) * riZ;
    if (tz_min > tz_max) { std::swap(tz_min, tz_max); }
    if (tx_min > tz_max || tz_min > tx_max) { return false; }
    return true;
}

std::ostream & operator<<(std::ostream & os, const BoundingBox & bbox) {
    os << "(" << bbox.x_min << "," << bbox.y_min << "," << bbox.z_min
       << ") -> (" << bbox.x_max << "," << bbox.y_max << "," << bbox.z_max
       << ")\n";
   return os;
}
