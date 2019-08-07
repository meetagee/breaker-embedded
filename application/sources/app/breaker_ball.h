#ifndef __BREAKER_BALL_H
#define __BREAKER_BALL_H__

#include "breaker_control.h"

// ball params
#define BALL_WIDTH			6
#define BALL_HEIGHT			6
#define MAX_DISPLACEMENT	2

extern coord_t ball;

extern void ballInit(void);
extern void moveBall(void);
extern bool checkLoss (void);

#endif // BREAKER_BALL_H
