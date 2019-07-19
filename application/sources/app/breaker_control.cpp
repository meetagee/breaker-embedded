#include "breaker_control.h"
#include "breaker_UI.h"
#include "task_list.h"
#include "app.h"
#include "app_dbg.h"
#include "timer.h"
#include <time.h>

brick_t bricks[ROW][COL];
coord_t ball;
coord_t paddle;
uint8_t lives, dir, dirPaddle;
static uint8_t tmp_lives;
bool controlReady, displayReady, gameOver;

// prototypes
void brickInit (void);
void ballInit (void);
void paddleInit (void);
void livesInit(void);
void dirInit (void);
void moveBall (void);
void detectCollision (void);
void movePaddle(void);
bool checkNewRound (void);

void task_game_control (ak_msg_t* msg) {
	switch(msg->sig) {
	case AC_GAME_CONTROL_INITIAL: {
		APP_DBG_SIG("AC_GAME_CONTROL_INITIAL\n");
		gameOver = false;
		brickInit();
		livesInit();

		paddleInit();
		ballInit();
		dirInit();

		controlReady = true;
		task_post_pure_msg(AC_TASK_GAME_CONTROL_ID, AC_GAME_CONTROL_PRE_PLAYING);
	}
		break;

	case AC_GAME_CONTROL_PRE_PLAYING: {
		APP_DBG_SIG("AC_GAME_CONTROL_PRE_PLAYING\n");
		if(controlReady == true && displayReady == true) {
			timer_remove_attr(AC_TASK_GAME_CONTROL_ID, AC_GAME_CONTROL_PRE_PLAYING);
			task_post_pure_msg(AC_TASK_GAME_CONTROL_ID, AC_GAME_CONTROL_PLAYING);
		}
		else { timer_set(AC_TASK_GAME_CONTROL_ID, AC_GAME_CONTROL_PRE_PLAYING, 1000, TIMER_ONE_SHOT); }
	}
		break;

	case AC_GAME_CONTROL_PLAYING: {
		APP_DBG_SIG("AC_GAME_CONTROL_PLAYING\n");
//		APP_DBG_SIG("lives = %d, tmp_lives = %d\n", lives, tmp_lives);
//		APP_DBG_SIG("checkNewRound() = %d\n", checkNewRound());
		if(gameOver == false) {
			movePaddle();
			moveBall();
			task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_START_GAME);
			detectCollision();
		}
		else {
			task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_GAME_OVER);
		}
	}
		break;

	case AC_GAME_CONTROL_NEW_ROUND: {
		APP_DBG_SIG("AC_GAME_CONTROL_NEW_ROUND\n");
		paddleInit();
		ballInit();
		dirInit();

		task_post_pure_msg(AC_TASK_GAME_CONTROL_ID, AC_GAME_CONTROL_PLAYING);
	}
		break;

	default:
		break;
	}
}

void brickInit (void) {
	bricks[0][0].coord.x = BRICK_START_X;
	bricks[0][0].coord.y = BRICK_START_Y;

	for(uint8_t i = 0; i < ROW; i++ ) {
		// First element of each row
		bricks[i][0].available = true;
		if(i != 0) {
			bricks[i][0].coord.x = bricks[0][0].coord.x;
			bricks[i][0].coord.y = bricks[i-1][0].coord.y + (BRICK_HEIGHT + 1);
		}
		// The rest of the elements of a row
		for(uint8_t j = 1; j < COL; j++) {
			bricks[i][j].coord.x = bricks[i][j-1].coord.x + (BRICK_WIDTH + 1);
			bricks[i][j].coord.y = bricks[i][j-1].coord.y;
			bricks[i][j].available = true;
		}
	}
}

void ballInit (void) {
	ball.x = paddle.x + PADDLE_WIDTH/2 - BALL_WIDTH/2;
	ball.y = paddle.y - BALL_HEIGHT;
}

void paddleInit (void) {
	paddle.x = GAME_BOX_X + (GAME_BOX_WIDTH/2 - PADDLE_WIDTH/2);
	paddle.y = GAME_BOX_Y + (GAME_BOX_HEIGHT - PADDLE_HEIGHT - 2);
	dirPaddle = PADDLE_STAY;
}

void livesInit (void) {
	lives = LIVES;
	tmp_lives = LIVES;
}

void dirInit (void) {
	dir = rand() %  1 ? UP_LEFT : UP_RIGHT;
}

void moveBall(void) {
	switch(dir) {
	case UP_LEFT: {
		ball.x -= 1;
		ball.y -= 1;
	}
		break;

	case UP_RIGHT: {
		ball.x += 1;
		ball.y -= 1;
	}
		break;

	case DOWN_LEFT: {
		ball.x -= 1;
		ball.y += 1;
	}
		break;

	case DOWN_RIGHT: {
		ball.x += 1;
		ball.y += 1;
	}
		break;
	}
}

void detectCollision (void) {
	switch(dir) {
	case (UP_RIGHT): {
		// Right wall
		if(ball.x + BALL_WIDTH >= GAME_BOX_X + GAME_BOX_WIDTH - 1 && ball.y - 1 > GAME_BOX_Y) { dir = UP_LEFT; }
		// Upper right corner
		else if (ball.x + BALL_WIDTH >= GAME_BOX_X + GAME_BOX_WIDTH - 1 && ball.y - 1 <= GAME_BOX_Y) { dir = DOWN_LEFT; }
		// Upper wall
		else if (ball.x + BALL_WIDTH < GAME_BOX_X + GAME_BOX_WIDTH - 1 && ball.y - 1 <= GAME_BOX_Y){ dir = DOWN_RIGHT; }

		// TODO: Hit brick

		else { dir = UP_RIGHT; }
	}
		break;

	case (UP_LEFT): {
		// Left wall
		if(ball.x - 1 <= GAME_BOX_X && ball.y - 1 > GAME_BOX_Y) { dir = UP_RIGHT; }
		// Upper left corner
		else if (ball.x - 1 <= GAME_BOX_X && ball.y - 1 <= GAME_BOX_Y) { dir = DOWN_RIGHT; }
		// Upper wall
		else if (ball.x - 1 > GAME_BOX_X && ball.y - 1 <= GAME_BOX_Y) { dir = DOWN_LEFT; }

		// TODO: Hit brick
		else { dir = UP_LEFT; };
	}
		break;

	case (DOWN_RIGHT): {
		// Right wall
		if(ball.x + BALL_WIDTH >= GAME_BOX_X + GAME_BOX_WIDTH - 1 && ball.y + BALL_HEIGHT < paddle.y) { dir = DOWN_LEFT; }

		// Hit paddle
		// Normal hit
		else if(((ball.x >= paddle.x && ball.x <= paddle.x + PADDLE_WIDTH - 1) || (ball.x + BALL_WIDTH - 1 >= paddle.x && ball.x + BALL_WIDTH - 1 <= paddle.x + PADDLE_WIDTH - 1)) && ball.y + BALL_HEIGHT == paddle.y ) { dir = UP_RIGHT; }
		// Corner hit
		else if(ball.x + 1 == paddle.x && ball.y + BALL_HEIGHT == paddle.y) { dir = UP_LEFT; }

//		/* FOR TESTING ONLY */
//		// Down right corner
//		else if(ball.x + BALL_WIDTH >= GAME_BOX_X + GAME_BOX_WIDTH - 1 && ball.y + BALL_HEIGHT >= GAME_BOX_Y + GAME_BOX_HEIGHT - 1) { dir = UP_LEFT; }
//		// Lower wall
//		else if(ball.x + BALL_WIDTH < GAME_BOX_X + GAME_BOX_WIDTH - 1 && ball.y + BALL_HEIGHT >= GAME_BOX_Y + GAME_BOX_HEIGHT - 1) { dir = UP_RIGHT; }

		// Loss
		else if(ball.y + BALL_HEIGHT > paddle.y ) { gameOver = true; }

		// TODO: Hit brick

		else { dir = DOWN_RIGHT; }
	}
		break;

	case (DOWN_LEFT): {
		// Left wall
		if(ball.x - 1 < GAME_BOX_X && ball.y + BALL_HEIGHT < paddle.y) { dir = DOWN_RIGHT; }

		// Hit paddle
		// Normal hit
		else if(((ball.x >= paddle.x && ball.x <= paddle.x + PADDLE_WIDTH - 1) || (ball.x + BALL_WIDTH - 1 >= paddle.x && ball.x + BALL_WIDTH - 1 <= paddle.x + PADDLE_WIDTH - 1)) && ball.y + BALL_HEIGHT == paddle.y ) { dir = UP_LEFT; }
		// Corner hit
		else if(ball.x - 1 == paddle.x + PADDLE_WIDTH - 1 && ball.y + BALL_HEIGHT == paddle.y) { dir = UP_RIGHT; }

//		/* FOR TESTING ONLY */
//		// Down left corner
//		else if(ball.x - 1 <= GAME_BOX_X && ball.y + BALL_HEIGHT >= GAME_BOX_Y + GAME_BOX_HEIGHT - 1) { dir = UP_RIGHT; }
//		// Lower wall
//		else if(ball.x + BALL_WIDTH < GAME_BOX_X + GAME_BOX_WIDTH - 1 && ball.y + BALL_HEIGHT >= GAME_BOX_Y + GAME_BOX_HEIGHT - 1) { dir = UP_LEFT; }

		// Loss
		else if(ball.y + BALL_HEIGHT > paddle.y ) { gameOver = true; }

		// TODO: Hit brick

		else { dir = DOWN_LEFT; }
	}
		break;

	default:
		break;
	}
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
		}
		else { paddle.x -= MAX_PADDLE_MOVE; }
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
		}
		else { paddle.x += MAX_PADDLE_MOVE; }
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

bool checkNewRound (void) {
	if(tmp_lives != lives) {
		tmp_lives = lives;
		return true;
	}
	else {
		return false;
	}
}
