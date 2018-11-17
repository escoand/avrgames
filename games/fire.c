#include <stdlib.h>
#include <string.h>

#if _WIN32
#include <windows.h>
#define ms_sleep(x) Sleep(x)
#else
#include <unistd.h>
#define ms_sleep(x) usleep(x * 1000)
#endif

#include "../output/output.h"
#include "fire.h"

void
fire_main(void)
{
    board_matrix    board;
    memset(&board, 0, sizeof(board));

    while (1) {
	for (uint8_t x = 0; x < BOARD_WIDTH; x++) {

	    // Step 1.  Cool down every cell a little
	    for (uint8_t y = 0; y < BOARD_HEIGHT; y++) {
		uint8_t         tmp =
		    rand() % (FIRE_COOLING * 10 / BOARD_HEIGHT + 2);
		board[y][x] = tmp < board[y][x] ? board[y][x] - tmp : 0;
	    }

	    // Step 2.  Heat from each cell drifts 'up' and diffuses a
	    // little
	    for (uint8_t y = BOARD_HEIGHT - 1; y >= 2; y--) {
		board[y][x] =
		    (board[y - 1][x] + board[y - 2][x] +
		     board[y - 2][x]) / 3;
	    }

	    // Step 3.  Randomly ignite new 'sparks' of heat near the
	    // bottom
	    if (rand() % 256 < FIRE_SPARKING) {
		uint8_t         y = rand() % 8;
		uint8_t         tmp = 160 + (rand() % 96);
		board[y][x] =
		    tmp + board[y][x] < 255 ? board[y][x] + tmp : 255;
	    }

	}

	setOutputUsePalette(&board, BOARD_PALETTE_FIRE);
	usleep(FIRE_TICK_MS * 1000);
    }
}
