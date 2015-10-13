#include "Color.hpp"

Color::Color() {

    red = green = blue = special = 0;

}

Color::Color(double red, double green, double blue, double special) {

    this->red = red;
    this->green = green;
    this->blue = blue;
    this->special = special;

}

double Color::getRed() const {

    return red;

}

double Color::getGreen() const {

    return green;

}

double Color::getBlue() const {

    return blue;

}

double Color::getSpecial() const {

    return special;

}

void Color::setRed(double r) {

    red = r;

}

void Color::setGreen(double g) {

    green = g;

}

void Color::setBlue(double b) {

    blue = b;

}

void Color::setSpecial(double s) {

    special = s;

}

std::ostream & operator<<(std::ostream & os, const Color & c) {

    os << "(Red: " << c.red << ", Green: " << c.green << ", Blue: " << c.blue
       << ", Special: " << c.special << ")";

    return os;

}
