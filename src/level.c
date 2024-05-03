#include "wasm4.h"

#include "level.h"
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
    text(textLives, 10, 10);
}

static void checkCollision(Level *level, Game *game) {
    // AABB screenLeft = { 0 };
    // aabbFromRect(-10, 0, 10, 160, &screenLeft);

    // AABB screenRight = { 0 };
    // aabbFromRect(160, 0, 10, 160, &screenRight);

    // AABB screenTop = { 0 };
    // aabbFromRect(0, -10, 160, 10, &screenTop);

    // AABB screenBottom = { 0 };
    // aabbFromRect(0, 160, 160, 10, &screenBottom);

    // AABB ballAABB;
    // aabbFromEntity(&entities[ENTITY_BALL], &ballAABB);

    // AABB padAABB;
    // aabbFromEntity(&entities[ENTITY_PAD], &padAABB);

    // AABB intersection = { 0 };

    // // pad vs screen left
    // if (aabbFindIntersection(&padAABB, &screenLeft, &intersection)) {
    //     entities[ENTITY_PAD].x += aabbWidth(&intersection);
    //     entities[ENTITY_PAD].vx *= -1;
    //     aabbFromEntity(&entities[ENTITY_PAD], &padAABB);
    // }

    // // pad vs screen right
    // if (aabbFindIntersection(&padAABB, &screenRight, &intersection)) {
    //     entities[ENTITY_PAD].x -= aabbWidth(&intersection);
    //     entities[ENTITY_PAD].vx *= -1;
    //     aabbFromEntity(&entities[ENTITY_PAD], &padAABB);
    // }

    // // ball vs screen left
    // if (aabbFindIntersection(&ballAABB, &screenLeft, &intersection)) {
    //     entities[ENTITY_BALL].x += aabbWidth(&intersection);
    //     entities[ENTITY_BALL].vx *= -1;
    //     aabbFromEntity(&entities[ENTITY_BALL], &ballAABB);
    // }

    // // ball vs screen right
    // if (aabbFindIntersection(&ballAABB, &screenRight, &intersection)) {
    //     entities[ENTITY_BALL].x -= aabbWidth(&intersection);
    //     entities[ENTITY_BALL].vx *= -1;
    //     aabbFromEntity(&entities[ENTITY_BALL], &ballAABB);
    // }

    // // ball vs screen top
    // if (aabbFindIntersection(&ballAABB, &screenTop, &intersection)) {
    //     entities[ENTITY_BALL].y += aabbHeight(&intersection);
    //     entities[ENTITY_BALL].vy *= -1;
    //     aabbFromEntity(&entities[ENTITY_BALL], &ballAABB);
    // }

    // // ball vs screen bottom
    // if (aabbFindIntersection(&ballAABB, &screenBottom, &intersection)) {
    //     entities[ENTITY_BALL].y -= aabbHeight(&intersection);
    //     entities[ENTITY_BALL].vy *= -1;
    //     aabbFromEntity(&entities[ENTITY_BALL], &ballAABB);

    //     lives--;
    //     if (lives == 0) {
    //         status = GAME_OVER;
    //     }

    //     tone(600 | (100 << 16), 15, 100, TONE_PULSE1);
    // }

    // // ball vs pad
    // if (aabbFindIntersection(&ballAABB, &padAABB, &intersection)) {
    //     entities[ENTITY_BALL].y -= aabbHeight(&intersection);
    //     entities[ENTITY_BALL].vy *= -1;
    //     aabbFromEntity(&entities[ENTITY_BALL], &ballAABB);
    // }

    // // ball vs blocks
    // for (int i = 0; i < GRID_TOTAL; ++i) {
    //     if (!entities[i].alive) {
    //         continue;
    //     }

    //     AABB blockAABB = { 0 };
    //     aabbFromEntity(&entities[i], &blockAABB);

    //     if (!aabbFindIntersection(&ballAABB, &blockAABB, &intersection)) {
    //         continue;
    //     }

    //     entities[i].alive = false;
    //     score++;
    //     if (score == GRID_TOTAL) {
    //         status = GAME_WON;
    //     }

    //     tone(100 | (500 << 16), 3, 100, TONE_PULSE2);

    //     AABB blockLeftAABB = { 0 };
    //     aabbFromRect(
    //         entities[i].x, entities[i].y, 1, entities[i].height,
    //         &blockLeftAABB
    //     );

    //     AABB blockRightAABB = { 0 };
    //     aabbFromRect(
    //         entities[i].x + entities[i].width - 1,
    //         entities[i].y,
    //         1,
    //         entities[i].height,
    //         &blockRightAABB
    //     );

    //     AABB blockTopAABB = { 0 };
    //     aabbFromRect(
    //         entities[i].x, entities[i].y, entities[i].width, 1, &blockTopAABB
    //     );

    //     AABB blockBottomAABB = { 0 };
    //     aabbFromRect(
    //         entities[i].x,
    //         entities[i].y + entities[i].height - 1,
    //         entities[i].width,
    //         1,
    //         &blockBottomAABB
    //     );

    //     // ball vs block left
    //     if (aabbFindIntersection(&ballAABB, &blockLeftAABB, &intersection) &&
    //         entities[ENTITY_BALL].vx > 0) {
    //         entities[ENTITY_BALL].x -= aabbWidth(&intersection);
    //         entities[ENTITY_BALL].vx *= -1;
    //         aabbFromEntity(&entities[ENTITY_BALL], &ballAABB);
    //     }

    //     // ball vs block right
    //     if (aabbFindIntersection(&ballAABB, &blockRightAABB, &intersection)
    //     &&
    //         entities[ENTITY_BALL].vx < 0) {
    //         entities[ENTITY_BALL].x += aabbWidth(&intersection);
    //         entities[ENTITY_BALL].vx *= -1;
    //         aabbFromEntity(&entities[ENTITY_BALL], &ballAABB);
    //     }

    //     // ball vs block top
    //     if (aabbFindIntersection(&ballAABB, &blockTopAABB, &intersection) &&
    //         entities[ENTITY_BALL].vy > 0) {
    //         entities[ENTITY_BALL].y -= aabbHeight(&intersection);
    //         entities[ENTITY_BALL].vy *= -1;
    //         aabbFromEntity(&entities[ENTITY_BALL], &ballAABB);
    //     }

    //     // ball vs block bottom
    //     if (aabbFindIntersection(&ballAABB, &blockBottomAABB, &intersection)
    //     &&
    //         entities[ENTITY_BALL].vy < 0) {
    //         entities[ENTITY_BALL].y += aabbHeight(&intersection);
    //         entities[ENTITY_BALL].vy *= -1;
    //         aabbFromEntity(&entities[ENTITY_BALL], &ballAABB);
    //     }

    //     break;
    // }
}
