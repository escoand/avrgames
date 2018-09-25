#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "output.h"


uint32_t output_colors[] = {
  ' ', '#', '$', '&', '=', '@', 'O'
};


void
initOutput (void)
{
}

void
setOutput (board_matrix * board)
{
#if _WIN32
  system ("cls");
#else
  system ("clear");
#endif
  printf ("+---------------------+\n");

  for (uint8_t y = 0; y < BOARD_HEIGHT; y++)
    {
      printf ("| ");
      for (uint8_t x = 0; x < BOARD_WIDTH; x++)
	{
	  printf ("%c ", output_colors[(*board)[y][x]]);
	}
      printf ("|\n");
    }

  printf ("+---------------------+\n");
}

void
clearOutput (void)
{
#if _WIN32
  system ("cls");
#else
  system ("clear");
#endif
}
