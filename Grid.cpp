#include "Grid.hpp"
#include "Ray3D.hpp"

#include <cassert>
#include <cmath>

Grid::Grid(const Vector3D & gridMin,
           const Vector3D & gridMax,
           const Vector3D & gridRes) {
    this->gridMin = gridMin;
    this->gridMax = gridMax;
    this->gridRes = gridRes;
    gridDim = gridMax - gridMin;
    assert(gridDim.getX() > 0);
    assert(gridDim.getY() > 0);
    assert(gridDim.getZ() > 0);
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
    double t_x, t_y, t_z;

    std::cout << "old t_x: " << (x_len - rayOrigin.getX()) / rayDir.getX() << "\n";
    std::cout << "old t_y: " << (y_len - rayOrigin.getY()) / rayDir.getY() << "\n";
    std::cout << "old t_z: " << (z_len - rayOrigin.getZ()) / rayDir.getZ() << "\n";
    Vector3D ogrid = rayOrigin - gridMin;
    double ocellx = ogrid.getX() / x_len;
    double ocelly = ogrid.getY() / y_len;
    double ocellz = ogrid.getZ() / z_len;
    if (rayDir.getX() >= 0) {
        t_x = ((floor(ocellx) + 1) * x_len - ogrid.getX()) / rayDir.getX();
    } else {
        t_x = ((floor(ocellx)    ) * x_len - ogrid.getX()) / rayDir.getX();
    }
    if (rayDir.getY() >= 0) {
        t_y = ((floor(ocelly) + 1) * y_len - ogrid.getY()) / rayDir.getY();
    } else {
        t_y = ((floor(ocelly)    ) * y_len - ogrid.getY()) / rayDir.getY();
    }
    if (rayDir.getZ() >= 0) {
        t_z = ((floor(ocellz) + 1) * z_len - ogrid.getZ()) / rayDir.getZ();
    } else {
        t_z = ((floor(ocellz)    ) * z_len - ogrid.getZ()) / rayDir.getZ();
    }
    std::cout << "new tx: " << t_x << "\n";
    std::cout << "new ty: " << t_y << "\n";
    std::cout << "new tz: " << t_z << "\n";
    Vector3D cellIndex(floor(rayOrigin.getX()),
                       floor(rayOrigin.getY()),
                       floor(rayOrigin.getZ()));
    // TODO Be sure to handle this index first before finding the next one
    double t = 0;
    int x_sign = getSign(rayDir.getX());
    int y_sign = getSign(rayDir.getY());
    int z_sign = getSign(rayDir.getZ());
    for (;;) {
        if (t_x < t_y) {
            if (t_z < t_x) {
                std::cout << "Aa\n";
                t = t_z;
                t_z += deltaT.getZ();
                cellIndex.setZ(cellIndex.getZ() + z_sign);
            } else if (t_x < t_z) {
                std::cout << "Bb\n";
                t = t_x;
                t_x += deltaT.getX();
                cellIndex.setX(cellIndex.getX() + x_sign);
            } else {
                std::cout << "Cc\n";
                t = t_x;
                t_x += deltaT.getX();
                t_z += deltaT.getZ();
                cellIndex.setX(cellIndex.getX() + x_sign);
                cellIndex.setZ(cellIndex.getZ() + z_sign);
            }
        } else if (t_y < t_x) {
            if (t_z < t_y) {
                std::cout << "Dd\n";
                t = t_z;
                t_z += deltaT.getZ();
                cellIndex.setZ(cellIndex.getZ() + z_sign);
            } else if (t_y < t_z) {
                std::cout << "Ee\n";
                t = t_y;
                t_y += deltaT.getY();
                cellIndex.setY(cellIndex.getY() + y_sign);
            } else {
                std::cout << "Ff\n";
                t = t_y;
                t_y += deltaT.getY();
                t_z += deltaT.getZ();
                cellIndex.setY(cellIndex.getY() + y_sign);
                cellIndex.setZ(cellIndex.getZ() + z_sign);
            }
        } else { // t_x = t_y
            if (t_x < t_z) {
                std::cout << "Gg\n";
                t = t_x;
                t_x += deltaT.getX();
                t_y += deltaT.getY();
                cellIndex.setX(cellIndex.getX() + x_sign);
                cellIndex.setY(cellIndex.getY() + y_sign);
            } else if (t_z < t_x) {
                std::cout << "Hh\n";
                t = t_z;
                t_z += deltaT.getZ();
                cellIndex.setZ(cellIndex.getZ() + z_sign);
            } else { // t_x = t_y = t_z
                std::cout << "Ii\n";
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
