#include "Color.hpp"

Color::Color(double r, double g, double b, double s) :
    red(r), green(g), blue(b), special(s) {}

double Color::getRed() const { return red; }

double Color::getGreen() const { return green; }

double Color::getBlue() const { return blue; }

double Color::getSpecial() const { return special; }

void Color::setRed(double r) { red = r; }

void Color::setGreen(double g) { green = g; }

void Color::setBlue(double b) { blue = b; }

void Color::setSpecial(double s) { special = s; }

std::ostream & operator<<(std::ostream & os, const Color & c) {
    os << "(Red: " << c.red << ", Green: " << c.green << ", Blue: " << c.blue
       << ", Special: " << c.special << ")";
    return os;
}

double Color::brightness() const { return (red + green + blue) / 3; }

Color Color::colorScalar(double scale) const {
    return Color(red * scale, green * scale, blue * scale, special);
}

Color operator+(const Color & lhs, const Color & rhs) {
    return Color(lhs.red + rhs.red,
                 lhs.green + rhs.green,
                 lhs.blue + rhs.blue,
                 lhs.special);
}

Color operator*(const Color & lhs, const Color & rhs) {
    return Color(lhs.red * rhs.red,
                 lhs.green * rhs.green,
                 lhs.blue * rhs.blue,
                 lhs.special);
}

Color Color::colorAverage(const Color & color) const {
    return Color((red + color.red) / 2,
                 (green + color.green) / 2,
                 (blue + color.blue) / 2,
                 special);
}

Color Color::clip() {
    double allLight = red + green + blue;
    double excessLight = allLight - 3;
    if (excessLight > 0) {
        red = red + excessLight * (red / allLight);
        green = green + excessLight * (green / allLight);
        blue = blue + excessLight * (blue / allLight);
    }
    if (red > 1) {
        red = 1;
    }
    if (green > 1) {
        green = 1;
    }
    if (blue > 1) {
        blue = 1;
    }
    if (red < 0) {
        red = 0;
    }
    if (green < 0) {
        green = 0;
    }
    if (blue < 0) {
        blue = 0;
    }
    return Color(red, green, blue, special);
}
