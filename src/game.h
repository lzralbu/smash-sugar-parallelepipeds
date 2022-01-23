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
    int maximumScore;

    int16_t score;
    int16_t lives;
} Game;

typedef enum GameState {
    GAME_WELCOME = 0,
    GAME_IN_PROGRESS = 1,
    GAME_WON         = 2,
    GAME_LOST        = 3
} GameState;

void Game_reset();
void Game_processInput();
void Game_update();
void Game_processCollision();
void Game_draw();

void Game_incrementScore();

void Game_incrementLives();
void Game_decrementLives();

GameState Game_getState();

#endif // _GAME_H
