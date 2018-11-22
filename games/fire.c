#include <stdlib.h>
#include <string.h>

#include "../env.h"
#include "../input/input.h"
#include "../output/output.h"
#include "fire.h"

void
fire_main(void)
{
    uint8_t         x,
                    y;
    board_matrix    board;
    memset(&board, 0, sizeof(board));

    // clear input
    ms_sleep(FIRE_TICK_MS);
    getInput();

    while (getInput() == BUTTON_NONE) {
	for (x = 0; x < BOARD_WIDTH; x++) {

	    // cool down
	    for (y = 0; y < BOARD_HEIGHT; y++) {
		uint8_t         tmp =
		    rand() % (FIRE_COOLING * 10 / BOARD_HEIGHT + 2);
		board[y][x] = tmp < board[y][x] ? board[y][x] - tmp : 0;
	    }

	    // drift up
	    for (y = BOARD_HEIGHT - 1; y >= 2; y--) {
		board[y][x] =
		    (board[y - 1][x] + board[y - 2][x] +
		     board[y - 2][x]) / 3;
	    }

	    // new fire
	    board[0][x] = FIRE_BURN_MIN + (rand() % (255 - FIRE_BURN_MIN));
	}

	setOutputUsePalette(&board, BOARD_PALETTE_FIRE);
	ms_sleep(FIRE_TICK_MS);
    }
}
