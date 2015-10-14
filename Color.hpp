#ifndef __COLOR__
#define __COLOR__

#include <iostream>

class Color {

    double red, green, blue, special;

    public:

        Color();
        Color(double red, double green, double blue, double special);
        double getRed() const;
        double getGreen() const;
        double getBlue() const;
        double getSpecial() const;

        void setRed(double r);
        void setGreen(double g);
        void setBlue(double b);
        void setSpecial(double s);

        friend std::ostream & operator<<(std::ostream & os, const Color & c);

        double brightness() const;
        Color colorScalar(double scale) const;

        friend Color operator+(const Color & lhs, const Color & rhs);
        friend Color operator*(const Color & lhs, const Color & rhs);

        Color colorAverage(const Color & color) const;
        Color clip();

};

#endif
