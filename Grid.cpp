#include "Grid.hpp"
#include "Ray3D.hpp"
#include "Object.hpp"
#include "Intersection.hpp"
#include "Helper.hpp"

#include <cassert>
#include <cmath>
#include <cfloat>
#include <vector>

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
    assert(x_len && y_len && z_len);
}

Grid::Grid(const std::vector<Object *> & scene_objects) {
    // Create bounding box by finding min and max of 3D points
    double maxX, maxY, maxZ, minX, minY, minZ;
    maxX = maxY = maxZ = -DBL_MAX;
    minX = minY = minZ = DBL_MAX;
    for (auto objPtr : scene_objects) {
        auto objBBox = objPtr->getBBox();
        maxX = std::max(maxX, objBBox.x_max);
        maxY = std::max(maxY, objBBox.y_max);
        maxZ = std::max(maxZ, objBBox.z_max);
        minX = std::min(minX, objBBox.x_min);
        minY = std::min(minY, objBBox.y_min);
        minZ = std::min(minZ, objBBox.z_min);
    }
    gridMin = Vector3D(minX, minY, minZ);
    gridMax = Vector3D(maxX, maxY, maxZ);
    bbox.set(minX, minY, minZ, maxX, maxY, maxZ);
    gridDim = gridMax - gridMin;
    assert(gridDim.getX() > 0);
    assert(gridDim.getY() > 0);
    assert(gridDim.getZ() > 0);
    double lambda = 4;
    double volume =
        fabs(gridDim.getX()) * fabs(gridDim.getY()) * fabs(gridDim.getZ());
    gridRes.setX(ceil(gridDim.getX() *
                      pow((lambda * scene_objects.size()) / volume,
                      1.0 / 3.0)));
    gridRes.setY(ceil(gridDim.getY() *
                      pow((lambda * scene_objects.size()) / volume,
                      1.0 / 3.0)));
    gridRes.setZ(ceil(gridDim.getZ() *
                      pow((lambda * scene_objects.size()) / volume,
                      1.0 / 3.0)));
    x_len = gridDim.getX() / gridRes.getX();
    y_len = gridDim.getY() / gridRes.getY();
    z_len = gridDim.getZ() / gridRes.getZ();
    matrix = std::vector<std::vector<std::vector<std::vector<Object *>>>>(
                gridRes.getX() + 1,
                std::vector<std::vector<std::vector<Object *>>>(
                    gridRes.getY() + 1,
                    std::vector<std::vector<Object *>>(
                        gridRes.getZ() + 1,
                        std::vector<Object *>())));
    for (auto x : matrix) {
        assert(x.size() == gridRes.getY() + 1);
        for (auto y : x) {
            assert(y.size() == gridRes.getZ() + 1);
            for (auto objs : y) {
                assert(objs.size() == 0);
            }
        }
    }
    assert(matrix.size() == gridRes.getX() + 1);
    // Store references to a triangle in all the cells its bounding box overlaps
    for (auto objPtr : scene_objects) {
        auto objBBox = objPtr->getBBox();
        Vector3D ogridmin =
            Vector3D(objBBox.x_min, objBBox.y_min, objBBox.z_min) - gridMin;
        Vector3D cellIndexMin(floor(ogridmin.getX() / x_len),
                              floor(ogridmin.getY() / y_len),
                              floor(ogridmin.getZ() / z_len));
        Vector3D ogridmax =
            Vector3D(objBBox.x_max, objBBox.y_max, objBBox.z_max) - gridMin;
        Vector3D cellIndexMax(floor(ogridmax.getX() / x_len),
                              floor(ogridmax.getY() / y_len),
                              floor(ogridmax.getZ() / z_len));
        for (double cix = cellIndexMin.getX();
              cix <= cellIndexMax.getX(); ++cix) {
            for (double ciy = cellIndexMin.getY();
                 ciy <= cellIndexMax.getY(); ++ciy) {
                for (double ciz = cellIndexMin.getZ();
                     ciz <= cellIndexMax.getZ(); ++ciz) {
                    matrix[cix][ciy][ciz].push_back(objPtr);
                }
            }
        }
    }
    for (auto x : matrix) {
        assert(x.size() == gridRes.getY() + 1);
        for (auto y : x) {
            assert(y.size() == gridRes.getZ() + 1);
        }
    }
    assert(matrix.size() == gridRes.getX() + 1);
}

int Grid::getSign(double d) const {
    return (d == 0 ? 0 : (d > 0 ? 1 : -1));
}

bool Grid::outOfBounds(const Vector3D & cellIndex) const {
    if (cellIndex.getX() < 0 || cellIndex.getX() > gridRes.getX() ||
        cellIndex.getY() < 0 || cellIndex.getY() > gridRes.getY() ||
        cellIndex.getZ() < 0 || cellIndex.getZ() > gridRes.getZ() ||
        isnan(cellIndex.getX()) || isnan(cellIndex.getY()) ||
        isnan(cellIndex.getZ())) {
        return true;
    }
    return false;
}

void Grid::findCellsIntersectedBy(const Ray3D & ray) {
    Vector3D rayDir = ray.getDirection();
    Vector3D rayOrigin = ray.getOrigin();
    rayDir.normalize();
    std::cout << "x_len: " << x_len << "\n";
    std::cout << "y_len: " << y_len << "\n";
    std::cout << "z_len: " << z_len << "\n";
    const Vector3D deltaT(std::abs(x_len / rayDir.getX()),
                          std::abs(y_len / rayDir.getY()),
                          std::abs(z_len / rayDir.getZ()));
    std::cout << "deltaT: " << deltaT << "\n";
    double t_x, t_y, t_z;
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
    // Check if origin falls in grid
    if (rayOrigin < gridMin || rayOrigin > gridMax) {
        std::cout << "Ray origin is outside grid\n"; return; }
    Vector3D cellIndex(floor(ocellx), floor(ocelly), floor(ocellz));
    int x_sign = getSign(rayDir.getX());
    int y_sign = getSign(rayDir.getY());
    int z_sign = getSign(rayDir.getZ());
    for (;;) {
        if (t_x < t_y) {
            if (t_z < t_x) {
                t_z += deltaT.getZ();
                cellIndex.setZ(cellIndex.getZ() + z_sign);
            } else if (t_x < t_z) {
                t_x += deltaT.getX();
                cellIndex.setX(cellIndex.getX() + x_sign);
            } else {
                t_x += deltaT.getX();
                t_z += deltaT.getZ();
                cellIndex.setX(cellIndex.getX() + x_sign);
                cellIndex.setZ(cellIndex.getZ() + z_sign);
            }
        } else if (t_y < t_x) {
            if (t_z < t_y) {
                t_z += deltaT.getZ();
                cellIndex.setZ(cellIndex.getZ() + z_sign);
            } else if (t_y < t_z) {
                t_y += deltaT.getY();
                cellIndex.setY(cellIndex.getY() + y_sign);
            } else {
                t_y += deltaT.getY();
                t_z += deltaT.getZ();
                cellIndex.setY(cellIndex.getY() + y_sign);
                cellIndex.setZ(cellIndex.getZ() + z_sign);
            }
        } else { // t_x = t_y
            if (t_x < t_z) {
                t_x += deltaT.getX();
                t_y += deltaT.getY();
                cellIndex.setX(cellIndex.getX() + x_sign);
                cellIndex.setY(cellIndex.getY() + y_sign);
            } else if (t_z < t_x) {
                t_z += deltaT.getZ();
                cellIndex.setZ(cellIndex.getZ() + z_sign);
            } else { // t_x = t_y = t_z
                t_x += deltaT.getX();
                t_y += deltaT.getY();
                t_z += deltaT.getZ();
                cellIndex.setX(cellIndex.getX() + x_sign);
                cellIndex.setY(cellIndex.getY() + y_sign);
                cellIndex.setZ(cellIndex.getZ() + z_sign);
            }
        }
        std::cout << cellIndex << std::endl;
        if (outOfBounds(cellIndex)) {
            break;
        }
    }
}

Intersection Grid::findIntersection(const Ray3D & ray, bool cast) const {
    Vector3D rayDir = ray.getDirection();
    rayDir.normalize();
    Vector3D rayOrigin = ray.getOrigin();
    // Check if origin falls in grid
    if (rayOrigin < gridMin || rayOrigin > gridMax) {
        auto r = bbox.intersects(ray);
        if (r) {
            rayOrigin = rayOrigin + rayDir * bbox.intersects(ray);
        } else {
            return Intersection();
        }
    }
    if (rayOrigin < gridMin || rayOrigin > gridMax) {
        assert(false);
    }
    const Vector3D deltaT(std::abs(x_len / rayDir.getX()),
                          std::abs(y_len / rayDir.getY()),
                          std::abs(z_len / rayDir.getZ()));
    double t_x, t_y, t_z;
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
    Vector3D cellIndex(floor(ocellx), floor(ocelly), floor(ocellz));
    assert(!outOfBounds(cellIndex));
    int x_sign = getSign(rayDir.getX());
    int y_sign = getSign(rayDir.getY());
    int z_sign = getSign(rayDir.getZ());
    Intersection bestSoFar;
    bestSoFar.distance = DBL_MAX;
    for (;;) {
        if (outOfBounds(cellIndex)) {
            break;
        }
        auto r =
            getIntersection(
                matrix[cellIndex.getX()][cellIndex.getY()][cellIndex.getZ()],
                ray);
        if (r.distance != -1 && r.distance < bestSoFar.distance) {
            bestSoFar.objPtr = r.objPtr;
            bestSoFar.distance = r.distance;
            if (!cast) {
                return bestSoFar;
            }
        }
        if (t_x < t_y) {
            if (t_z < t_x) {
                t_z += deltaT.getZ();
                cellIndex.setZ(cellIndex.getZ() + z_sign);
            } else if (t_x < t_z) {
                t_x += deltaT.getX();
                cellIndex.setX(cellIndex.getX() + x_sign);
            } else {
                t_x += deltaT.getX();
                t_z += deltaT.getZ();
                cellIndex.setX(cellIndex.getX() + x_sign);
                cellIndex.setZ(cellIndex.getZ() + z_sign);
            }
        } else if (t_y < t_x) {
            if (t_z < t_y) {
                t_z += deltaT.getZ();
                cellIndex.setZ(cellIndex.getZ() + z_sign);
            } else if (t_y < t_z) {
                t_y += deltaT.getY();
                cellIndex.setY(cellIndex.getY() + y_sign);
            } else {
                t_y += deltaT.getY();
                t_z += deltaT.getZ();
                cellIndex.setY(cellIndex.getY() + y_sign);
                cellIndex.setZ(cellIndex.getZ() + z_sign);
            }
        } else { // t_x = t_y
            if (t_x < t_z) {
                t_x += deltaT.getX();
                t_y += deltaT.getY();
                cellIndex.setX(cellIndex.getX() + x_sign);
                cellIndex.setY(cellIndex.getY() + y_sign);
            } else if (t_z < t_x) {
                t_z += deltaT.getZ();
                cellIndex.setZ(cellIndex.getZ() + z_sign);
            } else { // t_x = t_y = t_z
                t_x += deltaT.getX();
                t_y += deltaT.getY();
                t_z += deltaT.getZ();
                cellIndex.setX(cellIndex.getX() + x_sign);
                cellIndex.setY(cellIndex.getY() + y_sign);
                cellIndex.setZ(cellIndex.getZ() + z_sign);
            }
        }
    }
    if (bestSoFar.objPtr) {
        return bestSoFar;
    } else {
        return Intersection();
    }
}

void Grid::printIndicesContaining(Object * objPtr) const {
    bool found = false;
    for (double x = 0; x <= gridRes.getX(); ++x) {
        for (double y = 0; y <= gridRes.getY(); ++y) {
            for (double z = 0; z <= gridRes.getZ(); ++z) {
                for (auto obj : matrix[x][y][z]) {
                    if (obj == objPtr) {
                        found = true;
                        std::cout << x << " " << y << " " << z << "\n";
                    }
                }
            }
        }
    }
    if (!found) {
        std::cout << "Could not find " << objPtr << " in grid.\n";
    }
}
