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

#include "../output/output.h"
#include "qlock.h"


void
setWord(board_matrix *board, int8_t *word) {
  for(int8_t i = 0; i < BOARD_WIDTH + 1; i += 2) {
    if(word[i] < 0)
      break;
    (*board)[ word[i + 1] ][ word[i] ] = 1;
  }
}

void
qlock_main (void)
{
  int8_t qlockWords[19][BOARD_WIDTH * 2 + 1] = {
    { QLOCK_WORD_PREFIX, -1 },
    { QLOCK_WORD_BEFORE, -1 },
    { QLOCK_WORD_AFTER, -1 },
    { QLOCK_WORD_QUARTER, -1 },
    { QLOCK_WORD_HALF, -1 },
    { QLOCK_WORD_FORTYFIVE, -1 },
    { QLOCK_WORD_OCLOCK, -1 },
    { QLOCK_WORD_ONE, -1 },
    { QLOCK_WORD_TWO, -1 },
    { QLOCK_WORD_THREE, -1 },
    { QLOCK_WORD_FOUR, -1 },
    { QLOCK_WORD_FIVE, -1 },
    { QLOCK_WORD_SIX, -1 },
    { QLOCK_WORD_SEVEN, -1 },
    { QLOCK_WORD_EIGHT, -1 },
    { QLOCK_WORD_NINE, -1 },
    { QLOCK_WORD_TEN, -1 },
    { QLOCK_WORD_ELEVEN, -1 },
    { QLOCK_WORD_TWELVE, -1 }
  };

  board_matrix board;
  uint8_t hours;
  uint16_t mins;
  time_t rawtime;
  struct tm * timeinfo;

  initOutput ();

  while (1)
  {
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    hours = (timeinfo->tm_hour % 12);
    mins = (timeinfo->tm_min * 100) + timeinfo->tm_sec;

    // clear
    memset(&board, 0, sizeof(board));

    // prefix
    setWord(&board, qlockWords[0]);
#if DEBUG
    printf("%02i:%02i:%02i -> ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
#endif

    // shortly before
    if((mins % 1500) > 730 && (mins % 1500) <= 1230) {
      setWord(&board, qlockWords[1]);
#if DEBUG
      printf("shortly before ");
#endif
    }
    // shortly after
    else if((mins % 1500) > 230 && (mins % 1500) <= 730) {
      setWord(&board, qlockWords[2]);
#if DEBUG
      printf("shortly after ");
#endif
    }

    // quarter
    if(mins > 730 && mins <= 2230) {
      setWord(&board, qlockWords[3]);
#if QLOCK_QUARTER_AFTER == 0
      hours++;
#if DEBUG
      printf("quarter %i\n", hours);
#endif
#else
#if DEBUG
      printf("quarter past %i\n", hours);
#endif
#endif
    }
    // half
    else if(mins > 2230 && mins <= 3730) {
      setWord(&board, qlockWords[4]);
#if QLOCK_HALF_AFTER == 0
      hours++;
#if DEBUG
      printf("half to %i\n", hours);
#endif
#else
#if DEBUG
      printf("half past %i\n", hours);
#endif
#endif
    }
    // forty-five
    else if(mins > 3730 && mins <= 5230) {
      setWord(&board, qlockWords[5]);
#if QLOCK_FORTYFIVE_AFTER == 0
      hours++;
#if DEBUG
      printf("quarter to %i\n", hours);
#endif
#else
#if DEBUG
      printf("%i forty-five\n", hours);
#endif
#endif
    }
    // o'clock
    else if(mins > 5230) {
      setWord(&board, qlockWords[6]);
      hours++;
#if DEBUG
      printf("%i o'clock\n", hours);
#endif
    }
    else if(mins <= 730) {
      setWord(&board, qlockWords[6]);
#if DEBUG
      printf("%i o'clock\n", hours);
#endif
    }

    // hours
    setWord(&board, qlockWords[6 + hours]);

    setOutput(&board);
    sleep(10);
  }

  clearOutput ();
}
