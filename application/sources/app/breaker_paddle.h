#ifndef __BREAKER_PADDLE_H__
#define __BREAKER_PADDLE_H__

#include "breaker_control.h"
#include <stdint.h>

// paddle params
#define PADDLE_WIDTH        20
#define PADDLE_HEIGHT       2
#define PADDLE_STAY			0
#define PADDLE_LEFT			1
#define PADDLE_RIGHT		2
#define MAX_PADDLE_MOVE		5

extern coord_t paddle;
extern uint8_t dirPaddle;

extern void paddleInit (void);
extern void movePaddle (void);
extern void ballPaddleCollision (void);

#endif // BREAKER_PADDLE_H
