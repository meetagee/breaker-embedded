#ifndef __BREAKER_BRICKS_H__
#define __BREAKER_BRICKS_H__

#include "breaker_control.h"

// bricks params
#define ROW	5
#define COL	8
#define BRICK_WIDTH		10
#define BRICK_HEIGHT	4
#define BRICK_START_X	3
#define BRICK_START_Y	3

typedef struct {
    coord_t coord;
    bool available;
} brick_t;

extern brick_t bricks[ROW][COL];

extern void brickInit(void);
extern void ballBricksCollision (void);

#endif // BREAKER_BRICKS_H
