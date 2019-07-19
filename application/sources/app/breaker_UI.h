#ifndef __BREAKER_UI_H__
#define __BREAKER_UI_H__

#include "screen_manager.h"
#include "breaker_control.h"

#define GAME_BOX_WIDTH 88
#define GAME_BOX_HEIGHT 60
#define GAME_BOX_X 2
#define GAME_BOX_Y 2

#define SCORE_BOX_WIDTH 34
#define SCORE_BOX_HEIGHT 28
#define SCORE_BOX_X 92
#define SCORE_BOX_Y 2

#define LIVES_BOX_WIDTH 34
#define LIVES_BOX_HEIGHT 28
#define LIVES_BOX_X 92
#define LIVES_BOX_Y 34

extern scr_mng_t scr_mng_app;

extern void scr_startup_handle(ak_msg_t* msg);
extern void scr_start_game_handle(ak_msg_t* msg);

extern view_screen_t scr_startup;
extern view_screen_t scr_start_game;
#endif // __TASK_DISPLAY_H__
