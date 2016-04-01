#ifndef __AXIS__
#define __AXIS__

struct Axis {
    char a;
    Axis();
    Axis(char);
    Axis next() const;
};

#endif
