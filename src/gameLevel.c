#include "gameLevel.h"

#include "color.h"
#include "entity.h"
#include "input.h"
#include "utils.h"

#include "wasm4.h"

#include <math.h>

static bool paused = true;
static int8_t score = 0;
static int8_t lives = 3;

static Entity blocks[64] = { 0 };

// temporary buffs like a larger pad or one that fires bullets?
static Entity pad = { 0 };

// many balls, maybe?
static Entity ball = { 0 };

// ...just collisions for now!

static void ballReset(void);
static void checkCollisions(void);

void gameLevelStart(void) {
    score = 0;
    lives = 3;

    uint16_t blockWidth = 11;
    uint16_t horizontalSpace = 8;
    uint16_t blockHeight = 5;
    uint16_t verticalSpace = 3;
    for (uint16_t i = 0; i < 6; ++i) {
        for (uint16_t j = 0; j < 8; ++j) {
            Entity *block = &blocks[i * 8 + j];
            block->x =
                (float)(horizontalSpace + (blockWidth + horizontalSpace) * j);
            block->y =
                (float)(horizontalSpace + (blockHeight + verticalSpace) * i);
            block->vx = 0;
            block->vy = 0;
            block->width = blockWidth;
            block->height = 5;
            block->color = COLOR_3;
        }
    }

    pad.width = 30;
    pad.height = 1;
    pad.x = (160 - pad.width) / 2;
    pad.y = 160 - 2 * pad.height;
    pad.vx = 2;
    pad.vy = 0;
    pad.color = COLOR_2;

    ballReset();
}

void gameLevelUpdate(void) {
    if (isDown(1, BUTTON_LEFT)) {
        pad.x -= pad.vx;
    } else if (isDown(1, BUTTON_RIGHT)) {
        pad.x += pad.vx;
    }

    // if (paused && (isPressed(1, BUTTON_LEFT) || isPressed(1, BUTTON_RIGHT)))
    // {
    //     paused = false;
    //     ballReset();
    // }

    // if (!paused) {
    //     checkCollisions();
    // }

    for (size_t i = 0; i < 6 * 8; ++i) {
        entityUpdate(&blocks[i], false);
    }

    entityUpdate(&ball, true);

    entityUpdate(&pad, false);

    checkCollisions();
}

static void ballReset(void) {
    ball.width = ball.height = 2;
    ball.x = pad.x + pad.width / 2 - ball.width / 2;
    ball.y = pad.y - ball.height;
    ball.vx = -1.5f;
    ball.vy = -1.f;
    ball.color = COLOR_4;
}

static void checkCollisions(void) {
    float ballLeft = ball.x;
    float ballRight = ball.x + (float)ball.width;
    float ballTop = ball.y;
    float ballBottom = ball.y + (float)ball.height;

    // ball vs screen sides
    if ((ballLeft < 0 && ball.vx < 0) || (ballRight > 160 && ball.vx > 0)) {
        ball.vx *= -1;
    }

    // ball vs ceiling
    if (ballTop < 0 && ball.vy < 0) {
        ball.vy *= -1;
    }
    // ball vs floor
    // else if (ball.y > 160) {
    //     paused = true;
    //     ballReset();
    //     --lives;
    // }
    // temporary else
    else if (ballBottom > 160 && ball.vy > 0) {
        ball.vy *= -1;
    }

    // ball vs pad
    if ((ballBottom > pad.y && ball.vy > 0) &&
        (ballLeft >= pad.x && ballRight <= pad.x + pad.width)) {
        ball.vy *= -1;
    }

    // ball vs blocks
    for (size_t i = 0; i < 6 * 8; ++i) {
        float blockLeft = blocks[i].x;
        float blockRight = blocks[i].x + (float)blocks[i].width;
        float blockTop = blocks[i].y;
        float blockBottom = blocks[i].y + (float)blocks[i].height;

        bool touched = false;

        if ((ballLeft >= blockLeft && ballRight <= blockRight) &&
            ((ballBottom > blockTop && ball.vy > 0) ||
             (ballTop < blockBottom && ball.vy < 0))) {
            ball.vy *= -1;
            touched = true;
            blocks[i].color = COLOR_4;
        }

        if ((ballTop >= blockTop && ballBottom <= blockBottom) &&
            ((ballLeft < blockRight && ball.vx < 0) ||
             (ballRight > blockLeft && ball.vx > 0))) {
            ball.vx *= -1;
            touched = true;
            blocks[i].color = COLOR_4;
        }

        if (touched) {
            break;
        }
    }

    // pad vs screen sides
    if (pad.x < 0) {
        pad.x += pad.vx;
    } else if (pad.x + pad.width > 160) {
        pad.x -= pad.vx;
    }
}
