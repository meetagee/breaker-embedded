#ifndef __BREAKER_CONTROL_H__
#define __BREAKER_CONTROL_H__

#include <stdint.h>
#include "ak.h"

// bricks params
#define ROW	5
#define COL	8
#define BRICK_WIDTH		10
#define BRICK_HEIGHT	4
#define BRICK_START_X	3
#define BRICK_START_Y	3

// paddle params
#define PADDLE_WIDTH	20
#define PADDLE_HEIGHT	2

// ball params
#define BALL_WIDTH		6
#define BALL_HEIGHT		6
#define MAX_BALL_MOVE	6

// lives
#define LIVES	3

// dirs
#define UP_LEFT				1
#define UP_RIGHT			2
#define DOWN_LEFT			3
#define DOWN_RIGHT			4

#define PADDLE_STAY			0
#define PADDLE_LEFT			1
#define PADDLE_RIGHT		2
#define MAX_PADDLE_MOVE		5

typedef struct {
	uint8_t x;
	uint8_t y;
} coord_t;

typedef struct {
	coord_t coord;
	bool available;
} brick_t;

extern brick_t bricks[ROW][COL];
extern coord_t ball;
extern coord_t paddle;
extern uint8_t lives, dir, dirPaddle;
extern bool controlReady, displayReady, gameOver;


#endif // BREAKER_CONTROL_H
