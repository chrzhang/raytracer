#ifndef __BOUNDINGBOX__
#define __BOUNDINGBOX__

class Ray3D;

class BoundingBox {
    double x_min, y_min, z_min, x_max, y_max, z_max;
    public:
        BoundingBox();
        void set(double, double, double, double, double, double);
        bool intersects(const Ray3D &) const;
};

#endif
