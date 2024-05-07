#ifndef GAME_H
#define GAME_H

#include "menu.h"
#include "level.h"

typedef enum {
    GAME_STATE_MENU,
    GAME_STATE_ONGOING,
    GAME_STATE_WON,
    GAME_STATE_LOST
} GameStateFlag;

typedef struct {
    // union {
    //     Menu menu;
    //     Level level;
    // } data;

    Menu menu;
    Level level;

    GameStateFlag flag;
} Game;

void gameStart(Game *game);
void gameUpdate(Game *game);
void gameChangeState(Game *game, GameStateFlag newState);

#endif // GAME_H
