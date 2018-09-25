#ifdef __AVR__
#include <avr/io.h>
#endif
#include "games/menu.h"
#include "games/tetris.h"

int
main (void)
{
#ifdef CLKPR
  CLKPR = _BV (CLKPCE);
  CLKPR = 0;
#endif
  return menu_main ();
}
