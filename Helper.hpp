#include "Color.hpp"

Color randColor() {
    return Color( ((rand() % 100) / 100.0),
                  ((rand() % 100) / 100.0),
                  ((rand() % 100) / 100.0),
                  0 );
}
