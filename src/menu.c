#include "menu.h"

#include "animation.h"
#include "color.h"
#include "utils.h"

#include "../assets/images/controlsSpriteFrame1.h"
#include "../assets/images/controlsSpriteFrame2.h"
#include "../assets/images/controlsSpriteFrame3.h"
#include "../assets/images/sceneMenuSprite.h"

#include "wasm4.h"

static Animation controls;
static Animation footer;

void menuStart(void) {
    animationStart(&controls, 10, 1, 3, ANIMATION_PINGPONG);
    animationStart(&footer, 4, 1, 2, ANIMATION_FORWARD);
}

void menuUpdate(void) {
    *DRAW_COLORS = 0x4321;
    blit(
        sceneMenuSprite,
        0,
        0,
        sceneMenuSpriteWidth,
        sceneMenuSpriteHeight,
        sceneMenuSpriteFlags
    );

    setDrawColor(COLOR_SECONDARY, COLOR_4);

    int controlsX = 53;
    int controlsY = 115;

    animationUpdate(&controls);
    if (controls.current == 1) {
        blit(
            controlsSpriteFrame1,
            controlsX,
            controlsY,
            controlsSpriteFrame1Width,
            controlsSpriteFrame1Height,
            controlsSpriteFrame1Flags
        );
    } else if (controls.current == 2) {
        blit(
            controlsSpriteFrame2,
            controlsX,
            controlsY,
            controlsSpriteFrame2Width,
            controlsSpriteFrame2Height,
            controlsSpriteFrame2Flags
        );
    } else if (controls.current == 3) {
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

    animationUpdate(&footer);
    setDrawColor(COLOR_SECONDARY, COLOR_TRANSPARENT);
    if (footer.current == 1) {
        setDrawColor(COLOR_PRIMARY, COLOR_1);
    } else {
        setDrawColor(COLOR_PRIMARY, COLOR_TRANSPARENT);
    }
    rect(0, 135, 165, 30);
}
