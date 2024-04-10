#include "color.h"
#include "utils.h"

#include "wasm4.h"

#include <string.h>

void setDrawColor(int level, int color) {
    MY_ASSERT(COLOR_PRIMARY <= level && level <= COLOR_QUATERNARY);
    MY_ASSERT(COLOR_TRANSPARENT <= color && color <= COLOR_4);
    uint16_t offset = (uint16_t)(4 * level);
    uint16_t mask = 0xFFFF & ~(0xF << offset);
    *DRAW_COLORS = (uint16_t)((*DRAW_COLORS & mask) | (color << offset));
}

void clearScreen(int color) {
    MY_ASSERT(COLOR_1 <= color && color <= COLOR_4);
    --color;
    memset(
        FRAMEBUFFER,
        color | (color << 2) | (color << 4) | (color << 6),
        160 * 160 / 4
    );
}
