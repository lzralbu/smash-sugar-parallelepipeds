#ifndef _GAME_H
#define _GAME_H

#include "pad.h"
#include "ball.h"
#include "block.h"
#include "utils.h"

#define GAME_SCORE_X (SCREEN_SIZE - 9 * 8 - 1)
#define GAME_SCORE_Y 1
#define GAME_LIVES_X 1
#define GAME_LIVES_Y 1

typedef struct Game {
    Pad pad;
    Ball ball;
    Block grid[BLOCK_TOTAL_ROWS][BLOCK_TOTAL_COLUMNS];

    int16_t score;
    int16_t lives;
    Boolean paused;
} Game;

void Game_reset();
void Game_processInput();
void Game_update();
void Game_processCollision();
void Game_draw();

void Game_incrementScore();

#endif // _GAME_H
