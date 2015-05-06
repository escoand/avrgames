#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <light_ws2812.h>
#include "io.h"

void
output (struct cRGB board[BOARD_HEIGHT][BOARD_WIDTH])
{
  uint16_t x;
  uint16_t y;

  struct cRGB leds[BOARD_WIDTH * BOARD_HEIGHT];

  for (x = 0; x < BOARD_WIDTH; x++)
      for (y = 0; y < BOARD_HEIGHT; y++)
        {
    	leds[x * BOARD_HEIGHT + y].r = board[y][x].r;
  	leds[x * BOARD_HEIGHT + y].g = board[y][x].g;
	leds[x * BOARD_HEIGHT + y].b = board[y][x].b;
      }

  ws2812_setleds (leds, BOARD_WIDTH * BOARD_HEIGHT);
}

uint8_t
getKey (void)
{
  uint8_t key = 0;
  return key;
}
