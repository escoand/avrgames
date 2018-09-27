#include <stdint.h>
#include <string.h>

#if _WIN32
#include <windows.h>
#define ms_sleep(x) Sleep(x)
#else
#include <unistd.h>
#define ms_sleep(x) usleep(x * 1000)
#endif

#include "../output/output.h"
#include "loading.h"


void
loading_main(void)
{
    board_matrix    board;
    uint8_t         dots[] = { LOADING_DOTS };

    for (uint8_t i = 0; i < sizeof(dots) / sizeof(uint8_t); i += 2) {

	// clear
	memset(&board, 0, sizeof(board));

	// dots
	board[dots[i + 1]][dots[i]] = LOADING_DOT_COLOR;
	setOutput(&board);

	ms_sleep(LOADING_DOT_DELAY);
    }
}
