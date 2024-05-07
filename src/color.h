#ifndef COLOR_H
#define COLOR_H

typedef enum {
    COLOR_PRIMARY,
    COLOR_SECONDARY,
    COLOR_TERTIARY,
    COLOR_QUATERNARY,
} ColorLevel;

typedef enum {
    COLOR_TRANSPARENT,
    COLOR_1,
    COLOR_2,
    COLOR_3,
    COLOR_4,
} ColorCode;

void setDrawColor(int level, int colorCode);
void clearScreen(int colorCode);

#endif // COLOR_H
