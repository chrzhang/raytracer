#include "Helper.hpp"
#include "Color.hpp"

Color randColor() {
    return Color(0.5, 0.5, 0.5, 0);
    return Color( ((rand() % 100) / 100.0),
                  ((rand() % 100) / 100.0),
                  ((rand() % 100) / 100.0),
                  0 );
}
