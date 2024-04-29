#include "gameLevel.h"
#include "input.h"
#include "menu.h"

#include "wasm4.h"

enum {
    SCENE_MENU,
    SCENE_GAME_LEVEL,
};

static int currentScene = SCENE_MENU;

void start(void) {
    // DREAM CANDY PALETTE(https://lospec.com/palette-list/dream-candy)
    PALETTE[0] = 0x442d6e;
    PALETTE[1] = 0xd075b7;
    PALETTE[2] = 0xf0d063;
    PALETTE[3] = 0xffffff;

    *DRAW_COLORS = 0;

    menuStart();
}

void update(void) {
    inputUpdate();

    if (isAnyPressed(1) && currentScene == SCENE_MENU) {
        currentScene = SCENE_GAME_LEVEL;
        gameLevelStart();
    }

    if (currentScene == SCENE_MENU) {
        menuUpdate();
    } else if (currentScene == SCENE_GAME_LEVEL) {
        gameLevelUpdate();
    }
}
