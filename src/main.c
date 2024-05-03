#include "game.h"

#include "wasm4.h"

static Game game;

void start(void) {
    // DREAM CANDY PALETTE(https://lospec.com/palette-list/dream-candy)
    PALETTE[0] = 0x442d6e;
    PALETTE[1] = 0xd075b7;
    PALETTE[2] = 0xf0d063;
    PALETTE[3] = 0xffffff;

    *DRAW_COLORS = 0;

    gameStart(&game);
}

void update(void) {
    gameUpdate(&game);
}
