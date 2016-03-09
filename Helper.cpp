#include "Helper.hpp"
#include "Color.hpp"

Color randColor() {
    return Color(0.5, 0.5, 0.5, 0);
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
