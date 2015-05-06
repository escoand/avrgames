#include <avr/io.h>
#include "tetris.h"

int
main (void)
{
#ifdef CLKPR
  CLKPR = _BV(CLKPCE);
  CLKPR = 0;
#endif
  return tetris_main ();
}
