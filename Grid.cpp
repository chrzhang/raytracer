#include "Grid.hpp"
#include "Ray3D.hpp"

#include <cmath>

Grid::Grid(const Vector3D & gridDim,
           const Vector3D & gridRes) {
    this->gridDim = gridDim;
    this->gridRes = gridRes;
    x_len = gridDim.getX() / gridRes.getX();
    y_len = gridDim.getY() / gridRes.getY();
    z_len = gridDim.getZ() / gridRes.getZ();
}

int Grid::getSign(double d) {
    return (d == 0 ? 0 : (d > 0 ? 1 : -1));
}

bool Grid::outOfBounds(const Vector3D & cellIndex) {
    if (cellIndex.getX() < 0 || cellIndex.getX() >= gridDim.getX() ||
        cellIndex.getY() < 0 || cellIndex.getY() >= gridDim.getY() ||
        cellIndex.getZ() < 0 || cellIndex.getZ() >= gridDim.getZ()) {
        return true;
    }
    return false;
}

void Grid::findCellsIntersectedBy(const Ray3D & ray) {
    Vector3D rayDir = ray.getDirection();
    Vector3D rayOrigin = ray.getOrigin();
    rayDir.normalize();
    // Initial values
    const Vector3D deltaT(std::abs(x_len / rayDir.getX()),
                          std::abs(y_len / rayDir.getY()),
                          std::abs(z_len / rayDir.getZ()));
    double t_x = (x_len - rayOrigin.getX()) / rayDir.getX();
    double t_y = (y_len - rayOrigin.getY()) / rayDir.getY();
    double t_z = (z_len - rayOrigin.getZ()) / rayDir.getZ();
    Vector3D cellIndex(floor(rayOrigin.getX()),
                       floor(rayOrigin.getY()),
                       floor(rayOrigin.getZ()));
    double t = 0;
    int x_sign = getSign(rayDir.getX());
    int y_sign = getSign(rayDir.getY());
    int z_sign = getSign(rayDir.getZ());
    for (;;) {
        if (t_x < t_y) {
            if (t_z < t_x) {
                t = t_z;
                t_z += deltaT.getZ();
                cellIndex.setZ(cellIndex.getZ() + z_sign);
            } else if (t_x < t_z) {
                t = t_x;
                t_x += deltaT.getX();
                cellIndex.setX(cellIndex.getX() + x_sign);
            } else {
                t = t_x;
                t_x += deltaT.getX();
                t_z += deltaT.getZ();
                cellIndex.setX(cellIndex.getX() + x_sign);
                cellIndex.setZ(cellIndex.getZ() + z_sign);
            }
        } else if (t_y < t_x) {
            if (t_z < t_y) {
                t = t_z;
                t_z += deltaT.getZ();
                cellIndex.setZ(cellIndex.getZ() + z_sign);
            } else if (t_y < t_z) {
                t = t_y;
                t_y += deltaT.getY();
                cellIndex.setY(cellIndex.getY() + y_sign);
            } else {
                t = t_y;
                t_y += deltaT.getY();
                t_z += deltaT.getZ();
                cellIndex.setY(cellIndex.getY() + y_sign);
                cellIndex.setZ(cellIndex.getZ() + z_sign);
            }
        } else { // t_x = t_y
            if (t_x < t_z) {
                t = t_x;
                t_x += deltaT.getX();
                t_y += deltaT.getY();
                cellIndex.setX(cellIndex.getX() + x_sign);
                cellIndex.setY(cellIndex.getY() + y_sign);
            } else if (t_z < t_x) {
                t = t_z;
                t_z += deltaT.getZ();
                cellIndex.setZ(cellIndex.getZ() + z_sign);
            } else { // t_x = t_y = t_z
                t = t_x;
                t_x += deltaT.getX();
                t_y += deltaT.getY();
                t_z += deltaT.getZ();
                cellIndex.setX(cellIndex.getX() + x_sign);
                cellIndex.setY(cellIndex.getY() + y_sign);
                cellIndex.setZ(cellIndex.getZ() + z_sign);
            }
        }
        if (outOfBounds(cellIndex)) {
            break;
        }
        std::cout << cellIndex << std::endl;
    }
}
