#ifndef __HELPERRT__
#define __HELPERRT__

#include "Color.hpp"
#include "Intersection.hpp"
#include <vector>
#include "Object.hpp"
#include "Ray3D.hpp"

Color randColor();
bool stringMatches(const std::string &, const std::string &);
Intersection getIntersection(const std::vector<Object *> &,
                             const Ray3D &);

#endif
