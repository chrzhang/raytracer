#include "Grid.hpp"
#include "Ray3D.hpp"

void Grid::findCellsIntersectedBy(const Ray3D & ray) {
    Vector3D rayDir = ray.getDirection();
    rayDir.normalize();
    // Initial values
    const Vector3D deltaT = ray.getInvDirection();
    double t_x = deltaT.getX();
    double t_y = deltaT.getY();
    double t_z = deltaT.getZ();
    Vector3D cellIndex(0, 0, 0);
    double t = 0;
    for (int i = 0; i < 40; ++i) {
        if (t_x < t_y) {
            if (t_z < t_x) {
                t = t_z;
                t_z += deltaT.getZ();
                cellIndex.setZ(cellIndex.getZ() + 1);
            } else if (t_x < t_z) {
                t = t_x;
                t_x += deltaT.getX();
                cellIndex.setX(cellIndex.getX() + 1);
            } else {
                t = t_x;
                t_x += deltaT.getX();
                t_z += deltaT.getZ();
                cellIndex.setX(cellIndex.getX() + 1);
                cellIndex.setZ(cellIndex.getZ() + 1);
            }
        } else if (t_y < t_x) {
            if (t_z < t_y) {
                t = t_z;
                t_z += deltaT.getZ();
                cellIndex.setZ(cellIndex.getZ() + 1);
            } else if (t_y < t_z) {
                t = t_y;
                t_y += deltaT.getY();
                cellIndex.setY(cellIndex.getY() + 1);
            } else {
                t = t_y;
                t_y += deltaT.getY();
                t_z += deltaT.getZ();
                cellIndex.setY(cellIndex.getY() + 1);
                cellIndex.setZ(cellIndex.getZ() + 1);
            }
        } else { // t_x = t_y
            if (t_x < t_z) {
                t = t_x;
                t_x += deltaT.getX();
                t_y += deltaT.getY();
                cellIndex.setX(cellIndex.getX() + 1);
                cellIndex.setY(cellIndex.getY() + 1);
            } else if (t_z < t_x) {
                t = t_z;
                t_z += deltaT.getZ();
                cellIndex.setZ(cellIndex.getZ() + 1);
            } else { // t_x = t_y = t_z
                t = t_x;
                t_x += deltaT.getX();
                t_y += deltaT.getY();
                t_z += deltaT.getZ();
                cellIndex.setX(cellIndex.getX() + 1);
                cellIndex.setY(cellIndex.getY() + 1);
                cellIndex.setZ(cellIndex.getZ() + 1);
            }
        }
        std::cout << cellIndex << std::endl;
    }
}
