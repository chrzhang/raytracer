#include "Helper.hpp"
#include "Color.hpp"
#include <cfloat>

Color randColor() {
    //return Color(0.5, 0.5, 0.5, 0);
    return Color(0.5, 0.3, 0.9, 0.3);
    return Color( ((rand() % 100) / 100.0),
                  ((rand() % 100) / 100.0),
                  ((rand() % 100) / 100.0),
                  0 );
}

bool stringMatches(const std::string & s1, const std::string & s2) {
    if (s1.compare(s2) == 0) { return true; }
    std::cout << s1 << " does not match " << s2 << std::endl;
    return false;
}

Intersection getIntersection(const std::vector<Object *> & scene_objects,
                             const Ray3D & ray) {
    Object * objPtr = nullptr;
    double minDistance = DBL_MAX;
    int index = 0;
    for (auto sceneObjPtr : scene_objects) {
        double id = sceneObjPtr->findIntersection(ray);
        if (id >= 0 && id < minDistance) {
            objPtr = scene_objects[index];
            minDistance = id;
        }
        ++index;
    }
    if (objPtr && minDistance > 0) {
        return Intersection(objPtr, minDistance);
    } else {
        return Intersection();
    }
}
