#include "breaker_control.h"
#include "breaker_UI.h"
#include "breaker_ball.h"
#include "breaker_bricks.h"
#include "breaker_paddle.h"

#include "task_list.h"
#include "app.h"
#include "app_dbg.h"
#include "timer.h"

uint8_t lives, dir, dirPaddle;
bool gameStart;

// prototypes
void dirInit (void);
void detectCollision (void);
void livesInit(void);

void task_game_control (ak_msg_t* msg) {
	switch(msg->sig) {
	case AC_GAME_CONTROL_INITIAL: {
		APP_DBG_SIG("AC_GAME_CONTROL_INITIAL\n");
		dirInit();
		task_post_pure_msg(AC_TASK_PADDLE_ID, AC_PADDLE_INITIAL);
		task_post_pure_msg(AC_TASK_BALL_ID, AC_BALL_INITIAL);
		task_post_pure_msg(AC_TASK_BRICKS_ID, AC_BRICKS_INITIAL);
	}
		break;

	case AC_GAME_CONTROL_PRE_PLAYING: {
		APP_DBG_SIG("AC_GAME_CONTROL_PRE_PLAYING\n");
		if(!gameStart) {
			APP_DBG("gameStart: %d in CONTROL\n", gameStart);
			task_post_pure_msg(AC_TASK_GAME_CONTROL_ID, AC_GAME_CONTROL_PLAYING);
			gameStart = true;
		}
	}
		break;

	case AC_GAME_CONTROL_PLAYING: {
		//APP_DBG_SIG("AC_GAME_CONTROL_PLAYING\n");
		if(!checkLoss() && !checkWin()) {
			task_post_pure_msg(AC_TASK_BALL_ID, AC_BALL_PLAYING);
			task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_START_GAME);
			timer_set(AC_TASK_GAME_CONTROL_ID, AC_GAME_CONTROL_PLAYING, 50, TIMER_PERIODIC);
		}
		else {
			timer_remove_attr(AC_TASK_GAME_CONTROL_ID, AC_GAME_CONTROL_PLAYING);
		}
	}
		break;

	default:
		break;
	}
}

void dirInit (void) {
	dir = (rand() %  2 == 0) ? UP_LEFT : UP_RIGHT;
}

