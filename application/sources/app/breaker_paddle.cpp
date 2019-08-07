#include "breaker_paddle.h"
#include "breaker_ball.h"
#include "breaker_bricks.h"
#include "breaker_UI.h"

#include "app_dbg.h"
#include "app.h"
#include "task_list.h"

coord_t paddle;

// prototypes
void paddleInit (void);
void movePaddle (void);
void ballPaddleCollision (void);

void task_paddle_control(ak_msg_t* msg) {
	switch (msg->sig) {
	case(AC_PADDLE_INITIAL): {
		APP_DBG_SIG("AC_PADDLE_INITIAL\n");
		paddleInit();
	}
		break;

	case(AC_PADDLE_PLAYING): {
		APP_DBG_SIG("AC_PADDLE_PLAYING\n");
		movePaddle();
	}
		break;

	default:
		break;
	}
}

void paddleInit (void) {
    paddle.x = GAME_BOX_X + (GAME_BOX_WIDTH/2 - PADDLE_WIDTH/2);
    paddle.y = GAME_BOX_Y + (GAME_BOX_HEIGHT - PADDLE_HEIGHT - 2);
    dirPaddle = PADDLE_STAY;
}

void movePaddle (void) {
    switch(dirPaddle) {
    case PADDLE_LEFT: {
        if(paddle.x - MAX_PADDLE_MOVE < GAME_BOX_X) {
            for(uint8_t i = MAX_PADDLE_MOVE - 1; i > 0; i--) {
                if(paddle.x - i == GAME_BOX_X) {
                    paddle.x -= i;
                    break;
                }
            }
			dirPaddle = PADDLE_STAY;
        }
		else {
			paddle.x -= MAX_PADDLE_MOVE;
			dirPaddle = PADDLE_STAY;
		}
    }
        break;

    case PADDLE_RIGHT: {
        if(paddle.x + (PADDLE_WIDTH - 1) + MAX_PADDLE_MOVE > GAME_BOX_X + GAME_BOX_WIDTH - 1) {
            for(uint8_t i = MAX_PADDLE_MOVE - 1; i > 0; i--) {
                if(paddle.x + (PADDLE_WIDTH - 1) + i == GAME_BOX_X + GAME_BOX_WIDTH - 1) {
                    paddle.x += i;
                    break;
                }
            }
			dirPaddle = PADDLE_STAY;
        }
		else {
			paddle.x += MAX_PADDLE_MOVE;
			dirPaddle = PADDLE_STAY;
		}
    }
        break;

    case PADDLE_STAY: {
        paddle.x = paddle.x;
    }
        break;

    default:
        break;
    }
}

void ballPaddleCollision (void) {
	switch(dir) {
	case(DOWN_LEFT): {
		if (((ball.x >= paddle.x && ball.x < paddle.x + PADDLE_WIDTH - 1) || (ball.x + BALL_WIDTH - 1 >= paddle.x && ball.x + BALL_WIDTH - 1 < paddle.x + PADDLE_WIDTH - 1))
			&& (ball.y + BALL_HEIGHT == paddle.y))
			dir = UP_LEFT;
		else if ((ball.x == paddle.x + PADDLE_WIDTH)
				 && (ball.y + BALL_HEIGHT == paddle.y))
			dir = UP_RIGHT;
		else
			dir = DOWN_LEFT;
	}
		break;

	case(DOWN_RIGHT): {
		if (((ball.x >= paddle.x && ball.x < paddle.x + PADDLE_WIDTH - 1) || (ball.x + BALL_WIDTH - 1 >= paddle.x && ball.x + BALL_WIDTH - 1 < paddle.x + PADDLE_WIDTH - 1))
			&& (ball.y + BALL_HEIGHT == paddle.y))
			dir = UP_RIGHT;
		else if ((ball.x + BALL_WIDTH == paddle.x)
				 && (ball.y + BALL_HEIGHT == paddle.y))
			dir = UP_LEFT;
		else
			dir = DOWN_RIGHT;
	}
		break;

	default:
		break;
	}
}
