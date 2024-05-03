#include "wasm4.h"

#include "menu.h"
#include "game.h"
#include "animation.h"
#include "input.h"
#include "color.h"
#include "utils.h"

#include "../assets/images/controlsSpriteFrame1.h"
#include "../assets/images/controlsSpriteFrame2.h"
#include "../assets/images/controlsSpriteFrame3.h"
#include "../assets/images/sceneMenuSprite.h"

void menuStart(Menu *menu) {
    animationStart(&menu->controlsAnimation, 10, 1, 3, ANIMATION_PINGPONG);
    animationStart(&menu->footerAnimation, 4, 1, 2, ANIMATION_FORWARD);
}

void menuUpdate(Menu *menu, void *game) {
    if (isAnyPressed(1)) {
        gameChangeState(game, GAME_STATE_ONGOING);
    }

    animationUpdate(&menu->controlsAnimation);
    animationUpdate(&menu->footerAnimation);

    *DRAW_COLORS = 0x4321;
    blit(
        sceneMenuSprite,
        0,
        0,
        sceneMenuSpriteWidth,
        sceneMenuSpriteHeight,
        sceneMenuSpriteFlags
    );

    int controlsX = 53;
    int controlsY = 115;
    setDrawColor(COLOR_SECONDARY, COLOR_4);
    if (menu->controlsAnimation.current == 1) {
        blit(
            controlsSpriteFrame1,
            controlsX,
            controlsY,
            controlsSpriteFrame1Width,
            controlsSpriteFrame1Height,
            controlsSpriteFrame1Flags
        );
    } else if (menu->controlsAnimation.current == 2) {
        blit(
            controlsSpriteFrame2,
            controlsX,
            controlsY,
            controlsSpriteFrame2Width,
            controlsSpriteFrame2Height,
            controlsSpriteFrame2Flags
        );
    } else if (menu->controlsAnimation.current == 3) {
        blit(
            controlsSpriteFrame3,
            controlsX,
            controlsY,
            controlsSpriteFrame3Width,
            controlsSpriteFrame3Height,
            controlsSpriteFrame3Flags
        );
    }

    setDrawColor(COLOR_PRIMARY, COLOR_2);
    setDrawColor(COLOR_SECONDARY, COLOR_TRANSPARENT);
    text("Move", 53 + 16 + 6, 120);

    animationUpdate(&menu->footerAnimation);
    setDrawColor(COLOR_SECONDARY, COLOR_TRANSPARENT);
    if (menu->footerAnimation.current == 1) {
        setDrawColor(COLOR_PRIMARY, COLOR_1);
    } else {
        setDrawColor(COLOR_PRIMARY, COLOR_TRANSPARENT);
    }
    rect(0, 135, 165, 30);
}
