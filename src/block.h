#ifndef BLOCK_H
#define BLOCK_H

#include <stdint.h>

#define MAX_BLOCKS_TOTAL (15 * 15)

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t color;
} Block;

typedef struct {
    uint8_t blockWidth;
    uint8_t blockHeight;
    uint8_t cols;
    uint8_t rows;
    uint8_t hSpace;
    uint8_t vSpace;
    uint8_t numTotal;
    uint8_t numAlive;
    Block blocks[MAX_BLOCKS_TOTAL];
} BlockManager;

void blockManagerInit(
    BlockManager *manager, uint8_t blockWidth, uint8_t blockHeight,
    uint8_t cols, uint8_t rows, uint8_t hSpace, uint8_t vSpace
);
void blockManagerDraw(const BlockManager *manager);

void blockManagerActivateBlock(BlockManager *manager, uint8_t blockIndex);
void blockManagerDeactivateBlock(BlockManager *manager, uint8_t blockIndex);

#endif // BLOCK_H
