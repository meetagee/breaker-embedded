#ifndef __BREAKER_CONTROL_H__
#define __BREAKER_CONTROL_H__

#include <stdint.h>

// lives
#define LIVES	3

// dirs
#define UP_LEFT				1
#define UP_RIGHT			2
#define DOWN_LEFT			3
#define DOWN_RIGHT			4

typedef struct {
	uint8_t x;
	uint8_t y;
} coord_t;

extern uint8_t lives, dir;

#endif // BREAKER_CONTROL_H
