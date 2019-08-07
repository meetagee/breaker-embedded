#ifndef __BREAKER_CONTROL_H__
#define __BREAKER_CONTROL_H__

#include <stdint.h>

// dirs
#define UP_LEFT				1
#define UP_RIGHT			2
#define DOWN_LEFT			3
#define DOWN_RIGHT			4

typedef struct {
	int8_t x;
	int8_t y;
} coord_t;

extern bool gameStart;
extern uint8_t dir;

#endif // BREAKER_CONTROL_H
