#include <stdint.h>
#include <string.h>
#include <time.h>
#if _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#if DEBUG
#include <stdio.h>
#endif
#include <log.h>

#include "../output/output.h"
#include "clock.h"


uint8_t         clockDigits[][CLOCK_DIGIT_HEIGHT][CLOCK_DIGIT_WIDTH] = {
    {
     {1, 1, 1},
     {1, 0, 1},
     {1, 0, 1},
     {1, 0, 1},
     {1, 1, 1}
     },
    {
     {0, 1, 0},
     {1, 1, 0},
     {0, 1, 0},
     {0, 1, 0},
     {1, 1, 1}
     },
    {
     {1, 1, 1},
     {0, 0, 1},
     {1, 1, 1},
     {1, 0, 0},
     {1, 1, 1}
     },
    {
     {1, 1, 1},
     {0, 0, 1},
     {0, 1, 1},
     {0, 0, 1},
     {1, 1, 1}
     },
    {
     {1, 0, 1},
     {1, 0, 1},
     {1, 1, 1},
     {0, 0, 1},
     {0, 0, 1}
     },
    {
     {1, 1, 1},
     {1, 0, 0},
     {1, 1, 1},
     {0, 0, 1},
     {1, 1, 1}
     },
    {
     {1, 1, 1},
     {1, 0, 0},
     {1, 1, 1},
     {1, 0, 1},
     {1, 1, 1}
     },
    {
     {1, 1, 1},
     {0, 0, 1},
     {0, 0, 1},
     {0, 0, 1},
     {0, 0, 1}
     },
    {
     {1, 1, 1},
     {1, 0, 1},
     {1, 1, 1},
     {1, 0, 1},
     {1, 1, 1}
     },
    {
     {1, 1, 1},
     {1, 0, 1},
     {1, 1, 1},
     {0, 0, 1},
     {1, 1, 1}
     }
};

void
setDigit(board_matrix * board, uint8_t startX, uint8_t startY,
	 uint8_t digit[CLOCK_DIGIT_HEIGHT][CLOCK_DIGIT_WIDTH])
{
    for (uint8_t y = 0; y < CLOCK_DIGIT_HEIGHT; y++) {
	for (uint8_t x = 0; x < CLOCK_DIGIT_WIDTH; x++) {
	    (*board)[startY + y][startX + x] =
		digit[y][x] ? CLOCK_DIGIT_COLOR : 0;
	}
    }
}

uint8_t
clock_main(void)
{
    board_matrix    board;
    time_t          rawtime;
    struct tm      *timeinfo;
    uint8_t         hours,
                    mins,
                    secs;

    // check time
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // get infos
#if CLOCK_24_HOURS == 0
    hours = (timeinfo->tm_hour % 12);
#else
    hours = timeinfo->tm_hour;
#endif
    mins = timeinfo->tm_min;
    secs = timeinfo->tm_sec;

    log_debug("digits are %i %i %i %i", hours / 10, hours % 10, mins / 10,
	      mins % 10);

    // check if correct
    // if (timeinfo->tm_year < 100)
    if (access(CLOCK_CHECK_FILE, F_OK) == -1)
	return CLOCK_RETURN_FAILURE;

    // clear
    memset(&board, 0, sizeof(board));

    // digits
    setDigit(&board, CLOCK_DIGIT_COL1, CLOCK_DIGIT_ROW1,
	     clockDigits[hours / 10]);
    setDigit(&board, CLOCK_DIGIT_COL2, CLOCK_DIGIT_ROW1,
	     clockDigits[hours % 10]);
    setDigit(&board, CLOCK_DIGIT_COL1, CLOCK_DIGIT_ROW2,
	     clockDigits[mins / 10]);
    setDigit(&board, CLOCK_DIGIT_COL2, CLOCK_DIGIT_ROW2,
	     clockDigits[mins % 10]);

    setOutput(&board);

    return CLOCK_RETURN_SUCCESS;
}
