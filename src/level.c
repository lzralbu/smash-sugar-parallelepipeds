#include "wasm4.h"

#include "level.h"
#include "aabb.h"
#include "game.h"
#include "color.h"
#include "input.h"
#include "utils.h"

static void checkCollision(Level *level, Game *game);

void levelStart(Level *level) {
    blockManagerInit(&level->blockManager, 11, 5, 8, 5, 8, 2);
    ballInit(
        &level->ball,
        (160 - 2) / 2,
        160 - 20,
        2,
        2,
        0,
        0,
        -1.75f,
        -1.25f,
        COLOR_4
    );
    padInit(&level->pad, (160 - 30) / 2, 160 - 3, 30, 1, 0, 2.0f, 0, COLOR_2);
    level->lives = 3;
}

void levelUpdate(Level *level, void *game) {
    if (isDown(1, BUTTON_LEFT)) {
        level->pad.direction = -1;
    } else if (isDown(1, BUTTON_RIGHT)) {
        level->pad.direction = 1;
    } else {
        level->pad.direction = 0;
    }

    ballMove(&level->ball);
    padMove(&level->pad);

    checkCollision(level, game);

    if (level->lives == 0) {
        gameChangeState(game, GAME_STATE_LOST);
    }

    if (level->blockManager.numAlive == 0) {
        gameChangeState(game, GAME_STATE_WON);
    }

    blockManagerDraw(&level->blockManager);
    ballDraw(&level->ball);
    padDraw(&level->pad);

    char textLives[16] = "LIVES:";
    if (level->lives < 10) {
        textLives[7] = '0';
        itoa(textLives + 7, level->lives);
    } else {
        itoa(textLives + 6, level->lives);
    }
    textLives[15] = '\0';

    setDrawColor(COLOR_PRIMARY, COLOR_2);
    text(textLives, 10, 10);
}

static void checkCollision(Level *level, Game *game) {
    AABB screenLeft = { 0 };
    aabbFromRect(-10, 0, 10, 160, &screenLeft);

    AABB screenRight = { 0 };
    aabbFromRect(160, 0, 10, 160, &screenRight);

    AABB screenTop = { 0 };
    aabbFromRect(0, -10, 160, 10, &screenTop);

    AABB screenBottom = { 0 };
    aabbFromRect(0, 160, 160, 10, &screenBottom);

    AABB padAABB;
    aabbFromRect(
        level->pad.x, level->pad.y, level->pad.w, level->pad.h, &padAABB
    );

    AABB ballAABB;
    aabbFromRect(
        level->ball.x, level->ball.y, level->ball.w, level->ball.h, &ballAABB
    );

    AABB intersection = { 0 };

    // pad vs screen left
    if (aabbFindIntersection(&padAABB, &screenLeft, &intersection)) {
        level->pad.x += aabbWidth(&intersection);
        level->pad.direction *= -1;
        aabbFromRect(
            level->pad.x, level->pad.y, level->pad.w, level->pad.h, &padAABB
        );
    }

    // pad vs screen right
    if (aabbFindIntersection(&padAABB, &screenRight, &intersection)) {
        level->pad.x -= aabbWidth(&intersection);
        level->pad.direction *= -1;
        aabbFromRect(
            level->pad.x, level->pad.y, level->pad.w, level->pad.h, &padAABB
        );
    }

    // ball vs screen left
    if (aabbFindIntersection(&ballAABB, &screenLeft, &intersection)) {
        level->ball.x += aabbWidth(&intersection);
        level->ball.vx *= -1;
        aabbFromRect(
            level->ball.x,
            level->ball.y,
            level->ball.w,
            level->ball.h,
            &ballAABB
        );
    }

    // ball vs screen right
    if (aabbFindIntersection(&ballAABB, &screenRight, &intersection)) {
        level->ball.x -= aabbWidth(&intersection);
        level->ball.vx *= -1;
        aabbFromRect(
            level->ball.x,
            level->ball.y,
            level->ball.w,
            level->ball.h,
            &ballAABB
        );
    }

    // ball vs screen top
    if (aabbFindIntersection(&ballAABB, &screenTop, &intersection)) {
        level->ball.y += aabbHeight(&intersection);
        level->ball.vy *= -1;
        aabbFromRect(
            level->ball.x,
            level->ball.y,
            level->ball.w,
            level->ball.h,
            &ballAABB
        );
    }

    // ball vs screen bottom
    if (aabbFindIntersection(&ballAABB, &screenBottom, &intersection)) {
        level->ball.y -= aabbHeight(&intersection);
        level->ball.vy *= -1;
        aabbFromRect(
            level->ball.x,
            level->ball.y,
            level->ball.w,
            level->ball.h,
            &ballAABB
        );

        --level->lives;
        tone(600 | (100 << 16), 15, 100, TONE_PULSE1);
    }

    // ball vs pad
    if (aabbFindIntersection(&ballAABB, &padAABB, &intersection)) {
        level->ball.y -= aabbHeight(&intersection);
        level->ball.vy *= -1;
        aabbFromRect(
            level->ball.x,
            level->ball.y,
            level->ball.w,
            level->ball.h,
            &ballAABB
        );
    }

    // ball vs blocks
    for (int i = 0; i < level->blockManager.numAlive; ++i) {
        AABB blockAABB = { 0 };
        aabbFromRect(
            level->blockManager.blocks[i].x,
            level->blockManager.blocks[i].y,
            level->blockManager.blockWidth,
            level->blockManager.blockHeight,
            &blockAABB
        );

        if (!aabbFindIntersection(&ballAABB, &blockAABB, &intersection)) {
            continue;
        }

        blockManagerDeactivateBlock(&level->blockManager, (uint8_t)i);
        tone(100 | (500 << 16), 3, 100, TONE_PULSE2);

        AABB blockLeftAABB = { 0 };
        aabbFromRect(
            level->blockManager.blocks[i].x,
            level->blockManager.blocks[i].y,
            1,
            level->blockManager.blockHeight,
            &blockLeftAABB
        );

        AABB blockRightAABB = { 0 };
        aabbFromRect(
            level->blockManager.blocks[i].x + level->blockManager.blockWidth -
                1,
            level->blockManager.blocks[i].y,
            1,
            level->blockManager.blockHeight,
            &blockRightAABB
        );

        AABB blockTopAABB = { 0 };
        aabbFromRect(
            level->blockManager.blocks[i].x,
            level->blockManager.blocks[i].y,
            level->blockManager.blockWidth,
            1,
            &blockTopAABB
        );

        AABB blockBottomAABB = { 0 };
        aabbFromRect(
            level->blockManager.blocks[i].x,
            level->blockManager.blocks[i].y + level->blockManager.blockHeight -
                1,
            level->blockManager.blockWidth,
            1,
            &blockBottomAABB
        );

        // ball vs block left
        if (aabbFindIntersection(&ballAABB, &blockLeftAABB, &intersection) &&
            level->ball.vx > 0) {
            level->ball.x -= aabbWidth(&intersection);
            level->ball.vx *= -1;
            aabbFromRect(
                level->ball.x,
                level->ball.y,
                level->ball.w,
                level->ball.h,
                &ballAABB
            );
        }

        // ball vs block right
        if (aabbFindIntersection(&ballAABB, &blockRightAABB, &intersection) &&
            level->ball.vx < 0) {
            level->ball.x += aabbWidth(&intersection);
            level->ball.vx *= -1;
            aabbFromRect(
                level->ball.x,
                level->ball.y,
                level->ball.w,
                level->ball.h,
                &ballAABB
            );
        }

        // ball vs block top
        if (aabbFindIntersection(&ballAABB, &blockTopAABB, &intersection) &&
            level->ball.vy > 0) {
            level->ball.y -= aabbHeight(&intersection);
            level->ball.vy *= -1;
            aabbFromRect(
                level->ball.x,
                level->ball.y,
                level->ball.w,
                level->ball.h,
                &ballAABB
            );
        }

        // ball vs block bottom
        if (aabbFindIntersection(&ballAABB, &blockBottomAABB, &intersection) &&
            level->ball.vy < 0) {
            level->ball.y += aabbHeight(&intersection);
            level->ball.vy *= -1;
            aabbFromRect(
                level->ball.x,
                level->ball.y,
                level->ball.w,
                level->ball.h,
                &ballAABB
            );
        }

        break;
    }
}
