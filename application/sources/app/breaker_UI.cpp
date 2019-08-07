#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "sys_ctrl.h"
#include "sys_dbg.h"

#include "view_render.h"

#include "qrcode.h"

#include "app.h"
#include "app_dbg.h"

#include "task_list.h"

#include "breaker_UI.h"
#include "breaker_control.h"
#include "breaker_ball.h"
#include "breaker_bricks.h"
#include "breaker_paddle.h"

scr_mng_t scr_mng_app;

/* list of screen handler */
void scr_startup_handle(ak_msg_t* msg);
void scr_start_game_handle(ak_msg_t* msg);

/* list of view dynamic function */
static void view_scr_startup();
static void view_scr_start_game_frames();
static void view_scr_start_game_bricks();
static void view_scr_start_game_ball();
static void view_scr_start_game_paddle();

/**
 ******************************************************************************
 * objects MAIN SCREEN
 *
 ******************************************************************************
 */

/*----------[[[ITERM]]]------------*/

view_dynamic_t dyn_view_startup = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_startup
};

view_dynamic_t dyn_view_start_game_frames = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_start_game_frames
};

view_dynamic_t dyn_view_start_game_bricks = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_start_game_bricks
};

view_dynamic_t dyn_view_start_game_ball = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_start_game_ball
};

view_dynamic_t dyn_view_start_game_paddle = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_start_game_paddle
};


/*----------[[[OBJECT]]]------------*/
view_screen_t scr_startup = {
	&dyn_view_startup,
	ITEM_NULL,
	ITEM_NULL,
	ITEM_NULL,
	.focus_item = 0,
};

view_screen_t scr_start_game = {
	&dyn_view_start_game_frames,
	&dyn_view_start_game_bricks,
	&dyn_view_start_game_paddle,
	&dyn_view_start_game_ball,
	.focus_item = 0,
};

void task_display(ak_msg_t* msg) {
	scr_mng_dispatch(msg);
}

void scr_startup_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case AC_DISPLAY_INITIAL: {
		APP_DBG_SIG("AC_DISPLAY_INITIAL\n");
		view_render.initialize();
		view_render_display_off();
		SCREEN_NONE_UPDATE_MASK();
		task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_ON_LOGO);
	}
		break;

	case AC_DISPLAY_SHOW_ON_LOGO: {
		APP_DBG_SIG("AC_DISPLAY_SHOW_ON_LOGO\n");
		view_render_display_on();
	}
		break;

	case AC_DISPLAY_PRE_START_GAME: {
		APP_DBG_SIG("AC_DISPLAY_PRE_START_GAME\n");
		if(!gameStart) {
			APP_DBG("gameStart: %d in DISPLAY\n", gameStart);
			SCREEN_TRAN(scr_start_game_handle, &scr_start_game);
		}
	}
		break;

//	case AC_DISPLAY_SHOW_OFF: {
//		APP_DBG_SIG("AC_DISPLAY_SHOW_OFF\n");
//		view_render_display_off();
//		SCREEN_NONE_UPDATE_MASK();
//	}
//		break;

	default:
		break;
	}
}

void scr_start_game_handle(ak_msg_t* msg) {
	switch (msg->sig) {
	case AC_DISPLAY_START_GAME: {
		//APP_DBG_SIG("AC_DISPLAY_START_GAME\n");
	}
		break;

	case AC_DISPLAY_GAME_OVER: {
		APP_DBG_SIG("AC_DISPLAY_GAME_OVER\n");
	}
		break;

	default:
		break;
	}
}

void view_scr_startup() {
	view_render.fillScreen(BLACK);
	view_render.setTextSize(1);
	view_render.drawRect(2,2,124, 60, WHITE);
	view_render.setTextColor(WHITE);
	view_render.setCursor(40, 10);
	view_render.print("BREAKOUT");
	view_render.setCursor(7, 20);
	view_render.print("Press MODE to start");

	view_render.drawSun(47, 40, 10, WHITE);
	view_render.drawSun(57, 40, 10, WHITE);
	view_render.drawSun(67, 40, 10, WHITE);
}

void view_scr_start_game_frames() {
	view_render.fillScreen(BLACK);

	// game box
	view_render.drawRect(GAME_BOX_X,GAME_BOX_Y, GAME_BOX_WIDTH, GAME_BOX_HEIGHT, WHITE);

	// score box
	view_render.drawRect(SCORE_BOX_X, SCORE_BOX_Y, SCORE_BOX_WIDTH, SCORE_BOX_HEIGHT, WHITE);
	view_render.setCursor(95,4);
	view_render.print("SCORE");
	view_render.setCursor(100,14);
	view_render.print(score);
	view_render.drawLine(SCORE_BOX_X, 23, SCORE_BOX_X + SCORE_BOX_WIDTH - 1, 23, WHITE);
	view_render.setCursor(95, 20);
	view_render.print("  _  ");
	view_render.setCursor(95, 28);
	view_render.print("_(.)<");
	view_render.setCursor(95, 36);
	view_render.print("\\__) ");
	view_render.setCursor(100, 50);
	view_render.print("GO!");




	if(checkLoss()) {
		view_render.drawRect(4, 30, 84, 18 ,WHITE);
		view_render.setCursor(19, 35);
		view_render.print("GAME OVER!");
	}

	else if(checkWin()) {
		view_render.drawRect(4, 30, 84, 18 ,WHITE);
		view_render.setCursor(23, 35);
		view_render.print("YOU WIN!");
	}
}

void view_scr_start_game_bricks() {
	// bricks
	for(uint8_t i = 0; i < ROW; i++) {
		for(uint8_t j = 0; j < COL; j++) {
			if(bricks[i][j].available == true) {
				view_render.drawRect(bricks[i][j].coord.x,bricks[i][j].coord.y, BRICK_WIDTH, BRICK_HEIGHT, WHITE);
				view_render.fillRect(bricks[i][j].coord.x + 1, bricks[i][j].coord.y + 1, BRICK_WIDTH - 2, BRICK_HEIGHT - 2, WHITE);
			}
			else {
				view_render.drawRect(bricks[i][j].coord.x + 1 ,bricks[i][j].coord.y + 1, BRICK_WIDTH - 2, BRICK_HEIGHT - 2 , BLACK);
				view_render.fillRect(bricks[i][j].coord.x + 1, bricks[i][j].coord.y +1 , BRICK_WIDTH - 2 , BRICK_HEIGHT - 2, BLACK);
			}
		}
	}
}

void view_scr_start_game_ball() {
	// ball
	view_render.drawRect(ball.x, ball.y, BALL_WIDTH, BALL_HEIGHT, WHITE);
}

void view_scr_start_game_paddle() {
	// paddle
	view_render.drawRect(paddle.x, paddle.y, PADDLE_WIDTH, PADDLE_HEIGHT, WHITE);
}
