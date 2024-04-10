#ifndef COLOR_H
#define COLOR_H

enum ColorLevel {
    COLOR_PRIMARY,
    COLOR_SECONDARY,
    COLOR_TERTIARY,
    COLOR_QUATERNARY,
};

enum ColorCode {
    COLOR_TRANSPARENT,
    COLOR_1,
    COLOR_2,
    COLOR_3,
    COLOR_4,
};

void setDrawColor(int level, int colorCode);
void clearScreen(int colorCode);

#endif // COLOR_H
