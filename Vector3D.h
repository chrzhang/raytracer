#ifndef __VECTOR3D__
#define __VECTOR3D__

class Vector3D {

    double x, y, z;

    public:

        Vector3D();
        Vector3D(double x, double y, double z);
        double getX() const;
        double getY() const;
        double getZ() const;


};

#endif
