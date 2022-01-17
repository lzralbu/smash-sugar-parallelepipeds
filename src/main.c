#include "game.h"
#include "wasm4.h"

void start() {

    // ICE CREAM GB PALETTE(https://lospec.com/palette-list/ice-cream-gb)
    PALETTE[0] = 0xfff6d3;
    PALETTE[1] = 0xf9a875;
    PALETTE[2] = 0xeb6b6f;
    PALETTE[3] = 0x7c3f58;

    Game_reset();
}

void update () {

    Game_processInput();
    Game_update();
    Game_processCollision();
    Game_draw();
}
