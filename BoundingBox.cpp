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
double BoundingBox::intersects(const Ray3D & ray) const {
    double tx_min, tx_max, ty_min, ty_max, tz_min, tz_max;
    double roX = ray.getOrigin().getX();
    double roY = ray.getOrigin().getY();
    double roZ = ray.getOrigin().getZ();
    double riX = ray.getInvDirection().getX();
    double riY = ray.getInvDirection().getY();
    double riZ = ray.getInvDirection().getZ();
    if (riX < 0) {
        tx_min = (x_max - roX) * riX;
        tx_max = (x_min - roX) * riX;
    } else {
        tx_min = (x_min - roX) * riX;
        tx_max = (x_max - roX) * riX;
    }
    if (riY < 0) {
        ty_min = (y_max - roY) * riY;
        ty_max = (y_min - roY) * riY;
    } else {
        ty_min = (y_min - roY) * riY;
        ty_max = (y_max - roY) * riY;
    }
    if ((tx_min > ty_max) || (ty_min > tx_max)) {
        return 0;
    }
    if (ty_min > tx_min) {
        tx_min = ty_min;
    }
    if (ty_max < tx_max) {
        tx_max = ty_max;
    }
    if (riZ < 0) {
        tz_min = (z_max - roZ) * riZ;
        tz_max = (z_min - roZ) * riZ;
    } else {
        tz_min = (z_min - roZ) * riZ;
        tz_max = (z_max - roZ) * riZ;
    }
    if ((tx_min > tz_max) || (tz_min > tx_max)) {
        return 0;
    }
    if (tz_min > tx_min) {
        tx_min = tz_min;
    }
    if (tz_max < tx_max) {
        tx_max = tz_max;
    }
    if (tx_min < 0) {
        tx_min = tx_max;
        if (tx_min < 0) { return 0; }
    }
    // tx_min is the intersection
    return tx_min;
}

std::ostream & operator<<(std::ostream & os, const BoundingBox & bbox) {
    os << "(" << bbox.x_min << "," << bbox.y_min << "," << bbox.z_min
       << ") -> (" << bbox.x_max << "," << bbox.y_max << "," << bbox.z_max
       << ")\n";
   return os;
}
