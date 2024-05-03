#ifndef LEVEL_H
#define LEVEL_H

#include "block.h"
#include "ball.h"
#include "pad.h"

typedef struct {
    BlockManager blockManager;
    Ball ball;
    Pad pad;
    int lives;
} Level;

void levelStart(Level *level);
void levelUpdate(Level *level, void *game);

#endif // LEVEL_H
