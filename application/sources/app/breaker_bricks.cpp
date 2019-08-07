#include "breaker_bricks.h"
#include "breaker_ball.h"
#include "breaker_paddle.h"
#include "breaker_UI.h"

#include "app_dbg.h"
#include "app.h"
#include "task_list.h"

brick_t bricks[ROW][COL];
uint8_t bricksCount;
uint32_t score;

// prototypes
void brickInit (void);
void ballBricksCollision (void);
bool checkWin (void);

void task_bricks_control(ak_msg_t* msg) {
	switch (msg->sig) {
	case(AC_BRICKS_INITIAL): {
		APP_DBG_SIG("AC_BRICKS_INITIAL\n");
		brickInit();
	}
		break;

	case(AC_BRICKS_PLAYING): {
		APP_DBG_SIG("AC_BRICKS_PLAYING\n");
	}
		break;

	default:
		break;
	}
}

void brickInit (void) {
	bricksCount = 0;
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

void ballBricksCollision (void) {
	for(uint8_t i = 0; i < ROW; i++) {
		for(uint8_t j = 0; j < COL; j++) {
			if(bricks[i][j].available) {
				// Upper hit
				if(((ball.x >= bricks[i][j].coord.x && ball.x <= bricks[i][j].coord.x + BRICK_WIDTH - 1) || (ball.x + BALL_WIDTH - 1 >= bricks[i][j].coord.x && ball.x + BALL_WIDTH - 1 <= bricks[i][j].coord.x + BRICK_WIDTH - 1))
						&& (ball.y - MAX_DISPLACEMENT <= bricks[i][j].coord.y + BRICK_HEIGHT - 1 && ball.y - MAX_DISPLACEMENT >= bricks[i][j].coord.y)) {
					if(dir == UP_LEFT)
						dir = DOWN_LEFT;
					else
						dir = DOWN_RIGHT;
					bricks[i][j].available = false;
					bricksCount++;
					score += 10;
					break;
				}
				// Left hit
				else if (((ball.y >= bricks[i][j].coord.y && ball.y <= bricks[i][j].coord.y + BRICK_HEIGHT - 1) || (ball.y + BALL_HEIGHT - 1 >= bricks[i][j].coord.y && ball.y + BALL_HEIGHT - 1 <= bricks[i][j].coord.y + BRICK_HEIGHT - 1))
						 && (ball.x - MAX_DISPLACEMENT <= bricks[i][j].coord.x + BRICK_WIDTH - 1 && ball.x - MAX_DISPLACEMENT >= bricks[i][j].coord.x)) {
					if(dir == UP_LEFT)
						dir = UP_RIGHT;
					else
						dir = DOWN_RIGHT;
					bricks[i][j].available = false;
					bricksCount++;
					score += 10;
					break;
				}
				// Right hit
				else if (((ball.y >= bricks[i][j].coord.y && ball.y <= bricks[i][j].coord.y + BRICK_HEIGHT - 1) || (ball.y + BALL_HEIGHT - 1 >= bricks[i][j].coord.y && ball.y + BALL_HEIGHT - 1 <= bricks[i][j].coord.y + BRICK_HEIGHT - 1))
						 && (ball.x + BALL_WIDTH - 1 + MAX_DISPLACEMENT <= bricks[i][j].coord.x + BRICK_WIDTH - 1 && ball.x + BALL_WIDTH - 1 + MAX_DISPLACEMENT >= bricks[i][j].coord.x)) {
					if(dir == UP_RIGHT)
						dir = UP_LEFT;
					else
						dir = DOWN_LEFT;
					bricks[i][j].available = false;
					bricksCount++;
					score += 10;
					break;
				}
				// Lower hit
				else if(((ball.x >= bricks[i][j].coord.x && ball.x <= bricks[i][j].coord.x + BRICK_WIDTH - 1) || (ball.x + BALL_WIDTH - 1 >= bricks[i][j].coord.x && ball.x + BALL_WIDTH - 1 <= bricks[i][j].coord.x + BRICK_WIDTH - 1))
						&& (ball.y +BALL_HEIGHT - 1 + MAX_DISPLACEMENT <= bricks[i][j].coord.y + BRICK_HEIGHT - 1 && ball.y + BALL_HEIGHT - 1 + MAX_DISPLACEMENT >= bricks[i][j].coord.y)) {
					if(dir == DOWN_LEFT)
						dir = UP_LEFT;
					else
						dir = UP_RIGHT;
					bricks[i][j].available = false;
					bricksCount++;
					score += 10;
					break;
				}
				else {
					// Do nothing
				}
			}
		}
	}
}

bool checkWin(void) {
	if (bricksCount == ROW*COL) return true;
	else return false;
}
