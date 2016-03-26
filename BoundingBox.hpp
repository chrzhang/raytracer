#ifndef __BOUNDINGBOX__
#define __BOUNDINGBOX__

class BoundingBox {
    double x_min, y_min, z_min, x_max, y_max, z_max;
    public:
        BoundingBox() {
            x_min = y_min = z_min = x_max = y_max = z_max = 0;
        }
        void set(double xn, double yn, double zn, double xx, double yx,
                 double zx) {
            x_min = xn;
            y_min = yn;
            z_min = zn;
            x_max = xx;
            y_max = yx;
            z_max = zx;
        }
};

#endif
