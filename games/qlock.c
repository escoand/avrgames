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
#include "qlock.h"

int8_t          qlockWords[][BOARD_WIDTH * 2 + 1] = {
    {QLOCK_WORD_PREFIX, -1}
    ,
    {QLOCK_WORD_BEFORE, -1}
    ,
    {QLOCK_WORD_AFTER, -1}
    ,
    {QLOCK_WORD_QUARTER, -1}
    ,
    {QLOCK_WORD_HALF, -1}
    ,
    {QLOCK_WORD_FORTYFIVE, -1}
    ,
    {QLOCK_WORD_OCLOCK, -1}
    ,
    {QLOCK_WORD_ONE, -1}
    ,
    {QLOCK_WORD_TWO, -1}
    ,
    {QLOCK_WORD_THREE, -1}
    ,
    {QLOCK_WORD_FOUR, -1}
    ,
    {QLOCK_WORD_FIVE, -1}
    ,
    {QLOCK_WORD_SIX, -1}
    ,
    {QLOCK_WORD_SEVEN, -1}
    ,
    {QLOCK_WORD_EIGHT, -1}
    ,
    {QLOCK_WORD_NINE, -1}
    ,
    {QLOCK_WORD_TEN, -1}
    ,
    {QLOCK_WORD_ELEVEN, -1}
    ,
    {QLOCK_WORD_TWELVE, -1}
};

void
setWord(board_matrix * board, int8_t * word)
{
    for (int8_t i = 0; i < BOARD_WIDTH + 1; i += 2) {
	if (word[i] < 0)
	    break;
	(*board)[word[i + 1]][word[i]] = 1;
    }
}

void
qlock_main(void)
{
    board_matrix    board;
    uint8_t         hours;
    uint16_t        mins;
    time_t          rawtime;
    struct tm      *timeinfo;

    initOutput();

    while (1) {
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	hours = (timeinfo->tm_hour % 12);
	mins = (timeinfo->tm_min * 100) + timeinfo->tm_sec;

	// clear
	memset(&board, 0, sizeof(board));

	// prefix
	setWord(&board, qlockWords[0]);

	// shortly before
	if ((mins % 1500) > 730 && (mins % 1500) <= 1230) {
	    setWord(&board, qlockWords[1]);
	    log_debug("shortly before ");
	}
	// shortly after
	else if ((mins % 1500) > 230 && (mins % 1500) <= 730) {
	    setWord(&board, qlockWords[2]);
	    log_debug("shortly after ");
	}
	// quarter
	if (mins > 730 && mins <= 2230) {
	    setWord(&board, qlockWords[3]);
#if QLOCK_QUARTER_AFTER == 0
	    hours++;
	    log_debug("quarter %i2, hours);
#else
	    log_debug(" quarter past % i ", hours);
#endif
	}
	// half
	else if (mins > 2230 && mins <= 3730) {
	    setWord(&board, qlockWords[4]);
#if QLOCK_HALF_AFTER == 0
	    hours++;
	    log_debug(" half to % i ", hours);
#else
	    log_debug(" half past % i ", hours);
#endif
	}
	// forty-five
	else if (mins > 3730 && mins <= 5230) {
	    setWord(&board, qlockWords[5]);
#if QLOCK_FORTYFIVE_AFTER == 0
	    hours++;
	    log_debug(" quarter to % i ", hours);
#else
	    log_debug(" % i forty - five ", hours);
#endif
	}
	// o'clock
	else if (mins > 5230) {
	    setWord(&board, qlockWords[6]);
	    hours++;
	    log_debug(" % i o 'clock", hours);
	} else if (mins <= 730) {
	    setWord(&board, qlockWords[6]);
	    log_debug("%i o' clock ", hours);
	}
	// hours
	setWord(&board, qlockWords[6 + hours]);

	setOutput(&board);
	sleep(10);
    }

    clearOutput();
}


