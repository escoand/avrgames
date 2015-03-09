#include "output.h"
#include "tetris.h"

#ifdef __AVR__
#include <util/delay.h>
#define   usleep(x)  _delay_ms(200)
#elif _WIN32
#include <windows.h>
#define usleep(x) Sleep(x / 1000)
#else
#include <sys/time.h>
#endif


int
main (void)
{
  uint8_t key;
  uint16_t tick = 200 * 1000;
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
