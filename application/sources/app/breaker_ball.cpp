#include "breaker_ball.h"
#include "breaker_bricks.h"
#include "breaker_paddle.h"
#include "breaker_UI.h"

#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "timer.h"

coord_t ball;

// prototypes
void ballInit (void);
void moveBall (void);
void ballWallCollision(void);
bool checkLoss (void);

void task_ball_control(ak_msg_t* msg) {
	switch (msg->sig) {
	case(AC_BALL_INITIAL): {
		APP_DBG_SIG("AC_BALL_INITIAL\n");
		ballInit();
	}
		break;

	case(AC_BALL_PLAYING): {
		//APP_DBG_SIG("AC_BALL_PLAYING\n");
		if(!checkLoss() && !checkWin()) {
			moveBall();
			ballPaddleCollision();
			ballWallCollision();
			ballBricksCollision();
		}
	}
		break;

	default:
		break;
	}
}

void ballInit (void) {
    ball.x = paddle.x + PADDLE_WIDTH/2 - BALL_WIDTH/2;
    ball.y = paddle.y - BALL_HEIGHT;
}

void moveBall(void) {
    switch(dir) {
    case UP_LEFT: {
		ball.x -= MAX_DISPLACEMENT;
		ball.y -= MAX_DISPLACEMENT;
    }
        break;

    case UP_RIGHT: {
		ball.x += MAX_DISPLACEMENT;
		ball.y -= MAX_DISPLACEMENT;
    }
        break;

	case DOWN_LEFT: {
		ball.x -= MAX_DISPLACEMENT;
		ball.y += MAX_DISPLACEMENT;
    }
        break;

    case DOWN_RIGHT: {
		ball.x += MAX_DISPLACEMENT;
		ball.y += MAX_DISPLACEMENT;
    }
        break;

    default:
        break;
    }
}

void ballWallCollision(void) {
	switch(dir) {
	case(UP_LEFT): {
		if(ball.x - MAX_DISPLACEMENT <= GAME_BOX_X && ball.y - MAX_DISPLACEMENT > GAME_BOX_Y)
			dir = UP_RIGHT;
		else if(ball.x - MAX_DISPLACEMENT > GAME_BOX_X && ball.y - MAX_DISPLACEMENT <= GAME_BOX_Y )
			dir = DOWN_LEFT;
		else if(ball.x - MAX_DISPLACEMENT <= GAME_BOX_X && ball.y - MAX_DISPLACEMENT <= GAME_BOX_Y)
			dir = DOWN_RIGHT;
		else
			dir = UP_LEFT;
	}
		break;

	case(UP_RIGHT): {
		if(ball.x + BALL_WIDTH - 1 + MAX_DISPLACEMENT >= GAME_BOX_X + GAME_BOX_WIDTH - 1 && ball.y - MAX_DISPLACEMENT > GAME_BOX_Y)
			dir = UP_LEFT;
		else if(ball.x + BALL_WIDTH - 1 + MAX_DISPLACEMENT < GAME_BOX_X + GAME_BOX_WIDTH - 1 && ball.y - MAX_DISPLACEMENT <= GAME_BOX_Y)
			dir = DOWN_RIGHT;
		else if(ball.x + BALL_WIDTH - 1 + MAX_DISPLACEMENT >= GAME_BOX_X + GAME_BOX_WIDTH - 1 && ball.y - MAX_DISPLACEMENT >= GAME_BOX_Y)
			dir = DOWN_LEFT;
		else
			dir = UP_RIGHT;
	}
		break;

	case(DOWN_LEFT): {
		if(ball.x - MAX_DISPLACEMENT <= GAME_BOX_X && ball.y + BALL_HEIGHT <= paddle.y)
			dir = DOWN_RIGHT;
		else
			dir = DOWN_LEFT;
	}
		break;

	case(DOWN_RIGHT): {
		if(ball.x + BALL_WIDTH - 1 + MAX_DISPLACEMENT >= GAME_BOX_X + GAME_BOX_WIDTH - 1 && ball.y + BALL_HEIGHT <= paddle.y)
			dir = DOWN_LEFT;
		else
			dir = DOWN_RIGHT;
	}
		break;

	default:
		break;
	}
}

bool checkLoss (void) {
	if(ball.y + BALL_HEIGHT> paddle.y + PADDLE_HEIGHT - 1) return true;
	else return false;
}
