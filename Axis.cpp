#include "Axis.hpp"
#include <cassert>

Axis::Axis() : a('x') {}

Axis::Axis(char c) {
    switch (c) {
        case 'x':
        case 'y':
        case 'z':
            a = c;
            break;
        default:
            assert(0);
    }
}

Axis Axis::next() const {
    if (a == 'x') { return Axis('y'); }
    else if (a == 'y') { return Axis('z'); }
    else if (a == 'z') { return Axis('x'); }
    else { assert(0); }
}
