#include <string.h>
#include "output.h"
#include "tetris.h"

#ifdef __AVR__
#include <util/delay.h>
#define usleep(x) uint16_t y=x; for(;y>0;y--) _delay_ms(1);
#elif _WIN32
#include <windows.h>
#define usleep(x) Sleep(x)
#else
#include <sys/time.h>
#define usleep(x) usleep(x * 1000)
#endif


int
main (void)
{
  uint8_t key;
  uint16_t tick = 200;
  uint8_t board[BOARD_WIDTH][BOARD_HEIGHT];

  memset (board, 0, sizeof (board));

  while (1)
    {
      initOutput ();
      key = getKey ();
      if (!nextStep (board, key))
	return 1;
      output (board);
      usleep (tick);
    }

  return 0;
}
