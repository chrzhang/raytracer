#include "BVH.hpp"
#include "Object.hpp"
#include "Triangle.hpp"
#include "Helper.hpp"

#include <algorithm>
#include <vector>
#include <cfloat>
#include <cassert>

#define LEAF_THRESHOLD 5

BVH::BVH(const std::vector<Object *> & scene_objects, const Axis & axis) {
    // Create bounding box by finding min and max of 3D points
    double maxX, maxY, maxZ, minX, minY, minZ, pivot;
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
    bbox.set(minX, minY, minZ, maxX, maxY, maxZ);
    //std::cout << "Number of objects: " << scene_objects.size() << std::endl;
    // If number of objects is under a threshold, turn into leaf, split no more
    if (scene_objects.size() <= LEAF_THRESHOLD) {
        objsIncluded = scene_objects;
        left = right = nullptr;
        return;
    }
    // Use min and max to get a pivot along chosen axis
    switch (axis.a) {
        case 'x': {
            pivot = (maxX + minX) / 2;
            break;
        }
        case 'y': {
            pivot = (maxY + minY) / 2;
            break;
        }
        case 'z': {
            pivot = (maxZ + minZ) / 2;
            break;
        }
        default: {
            assert(0);
            break;
        }
    }
    std::vector<Object *> scene_objects_left, scene_objects_right;
    // Partition primitives according to pivot
    for (auto objPtr : scene_objects) {
        Vector3D centroid = dynamic_cast<Triangle *>(objPtr)->getCentroid();
        switch (axis.a) {
            case 'x': {
                if (pivot > centroid.getX()) {
                    scene_objects_left.push_back(objPtr);
                } else {
                    scene_objects_right.push_back(objPtr);
                }
                break;
            }
            case 'y': {
                if (pivot > centroid.getY()) {
                    scene_objects_left.push_back(objPtr);
                } else {
                    scene_objects_right.push_back(objPtr);
                }
                break;
            }
            case 'z': {
                if (pivot > centroid.getZ()) {
                    scene_objects_left.push_back(objPtr);
                } else {
                    scene_objects_right.push_back(objPtr);
                }
                break;
            }
        }
    }
    assert(scene_objects_left.size() + scene_objects_right.size() ==
           scene_objects.size());
    // Set left and right to BVHs of the left and right half of the sorted
    if (scene_objects_left.empty()) {
        left = nullptr;
    } else {
        left = new BVH(scene_objects_left, axis.next());
    }
    if (scene_objects_right.empty()) {
        right = nullptr;
    } else {
        right = new BVH(scene_objects_right, axis.next());
    }
}

Intersection BVH::findIntersection(const Ray3D & ray) const {
    if (!bbox.intersects(ray)) { return Intersection(); }
    if (!objsIncluded.empty()) { // Leaf
        assert(objsIncluded.size() <= LEAF_THRESHOLD);
        return getIntersection(objsIncluded, ray);
    }
    Intersection lookLeft, lookRight;
    if (left) { lookLeft = left->findIntersection(ray); }
    if (right) { lookRight = right->findIntersection(ray); }
    if (lookLeft.objPtr && lookRight.objPtr) {
        return (lookLeft.distance < lookRight.distance ? lookLeft : lookRight);
    }
    if (lookLeft.objPtr) {
        return lookLeft;
    }
    if (lookRight.objPtr) {
        return lookRight;
    }
    return Intersection();
}

void BVH::print() const {
}

BVH::~BVH() {
    if (left) {
        delete left;
    }
    if (right) {
        delete right;
    }
}
