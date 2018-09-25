#include <stdint.h>
#include <unistd.h>

#ifdef __AVR__
#include <avr/io.h>
#endif

#include "input/input.h"
#include "output/output.h"
#include "games/qlock.h"
#include "games/tetris.h"

uint8_t title[BOARD_HEIGHT][BOARD_WIDTH] = {
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 2, 0, 0, 0, 4, 4, 0, 0, 0},
   {0, 2, 0, 0, 3, 3, 4, 4, 0, 0},
   {0, 2, 0, 0, 3, 0, 3, 3, 3, 0},
   {0, 2, 0, 0, 3, 0, 3, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 1, 0, 0, 0, 2, 2, 2, 0, 0},
   {0, 1, 0, 0, 2, 0, 0, 0, 2, 0},
   {0, 1, 0, 0, 2, 0, 3, 3, 2, 0},
   {0, 1, 0, 0, 2, 0, 3, 0, 2, 0},
   {0, 1, 0, 0, 0, 2, 2, 2, 0, 0}
};

int
main (void)
{
  uint32_t button;

#ifdef CLKPR
  CLKPR = _BV (CLKPCE);
  CLKPR = 0;
#endif

  initInput();
  initOutput();
  setOutput (&title);

  while(1) {
    button = getInput();

    if(button == BUTTON_UP)
      qlock_main();
    else if(button == BUTTON_LEFT)
      tetris_main();
    else
      sleep(1);
  }

  clearOutput ();
}
