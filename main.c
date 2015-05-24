#ifdef __AVR__
#include <avr/io.h>
#endif
#include "tetris.h"

int
main (void)
{
#ifdef CLKPR
  CLKPR = _BV (CLKPCE);
  CLKPR = 0;
#endif
  return tetris_main ();
}
