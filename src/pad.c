#include "wasm4.h"

#include "pad.h"
#include "color.h"

void padInit(
    Pad *pad, int16_t x, int16_t y, int16_t w, int16_t h, int16_t speed,
    uint8_t color
) {
    pad->body.position[0] = x;
    pad->body.position[1] = y;
    pad->body.velocity[0] = speed;
    pad->body.velocity[1] = 0;
    pad->rect.body = &pad->body;
    pad->rect.d[0] = w;
    pad->rect.d[1] = h;
    pad->color = color;
    pad->moving = false;
}

void padMove(Pad *pad) {
    if (pad->moving) {
        pad->body.position[0] += pad->body.velocity[0];
    }
}

void padDraw(const Pad *pad) {
    setDrawColor(COLOR_PRIMARY, pad->color);
    rect(
        pad->body.position[0],
        pad->body.position[1],
        (uint32_t)pad->rect.d[0],
        (uint32_t)pad->rect.d[1]
    );
}
