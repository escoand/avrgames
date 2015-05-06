#include <avr/io.h>
#include <light_ws2812.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
//#include "io.h"
#include "tetris.h"

//#define ms_sleep(x) {uint16_t ms; for(ms=x;ms>0;ms--) _delay_ms(1);}
#define ms_sleep(x) _delay_ms(1000)

struct cRGB colors[7];

uint8_t bricks[][TETRIS_BRICK_SIZE][TETRIS_BRICK_SIZE] = {
  {
   {0, 1, 0, 0}
   ,
   {0, 1, 0, 0}
   ,
   {0, 1, 0, 0}
   ,
   {0, 1, 0, 0}
   }
  ,
  {
   {0, 0, 0, 0}
   ,
   {0, 0, 1, 0}
   ,
   {0, 0, 1, 0}
   ,
   {0, 1, 1, 0}
   }
  ,
  {
   {0, 0, 0, 0}
   ,
   {0, 1, 0, 0}
   ,
   {0, 1, 0, 0}
   ,
   {0, 1, 1, 0}
   }
  ,
  {
   {0, 0, 0, 0}
   ,
   {0, 1, 1, 0}
   ,
   {0, 1, 1, 0}
   ,
   {0, 0, 0, 0}
   }
  ,
  {
   {0, 0, 0, 0}
   ,
   {0, 1, 0, 0}
   ,
   {0, 1, 1, 0}
   ,
   {0, 0, 1, 0}
   }
  ,
  {
   {0, 0, 0, 0}
   ,
   {0, 0, 1, 0}
   ,
   {0, 1, 1, 0}
   ,
   {0, 1, 0, 0}
   }
  ,
  {
   {0, 0, 0, 0}
   ,
   {0, 1, 0, 0}
   ,
   {0, 1, 1, 0}
   ,
   {0, 1, 0, 0}
   }
};

struct cRGB board[BOARD_HEIGHT][BOARD_WIDTH];
struct cRGB brick[TETRIS_BRICK_SIZE][TETRIS_BRICK_SIZE];
int16_t offset_x = INT16_MAX;
int16_t offset_y = INT16_MAX;

uint16_t tick = 200;

uint8_t
insertBrick (int16_t _offset_x, int16_t _offset_y, enum tetris_actions action)
{
  uint8_t x, y;
  struct cRGB tmp[TETRIS_BRICK_SIZE][TETRIS_BRICK_SIZE];

  /* action */
  if (action == ROTATE_LEFT || action == ROTATE_RIGHT)
    {
      for (x = 0; x < TETRIS_BRICK_SIZE; x++)
	for (y = 0; y < TETRIS_BRICK_SIZE; y++)
	  {
	    if (action == ROTATE_RIGHT) {
              tmp[x][TETRIS_BRICK_SIZE - y - 1].r = brick[y][x].r;
              tmp[x][TETRIS_BRICK_SIZE - y - 1].g = brick[y][x].g;
              tmp[x][TETRIS_BRICK_SIZE - y - 1].b = brick[y][x].b;
}

	    else if (action == ROTATE_LEFT) {
              tmp[x][TETRIS_BRICK_SIZE - x - 1].r = brick[y][x].r;
              tmp[x][TETRIS_BRICK_SIZE - x - 1].g = brick[y][x].g;
              tmp[x][TETRIS_BRICK_SIZE - x - 1].b = brick[y][x].b;
}
	  }
    }
  else
    memcpy (tmp, brick, sizeof (tmp));

  /* check */
  if (action != REVERSE)
    for (x = 0; x < TETRIS_BRICK_SIZE; x++)
      {
	for (y = 0; y < TETRIS_BRICK_SIZE; y++)
	  {
	    if (_offset_y + y < 0)
	      continue;

	    else if ((tmp[y][x].r || tmp[y][x].g || tmp[y][x].b)
		     && (_offset_y + y >= BOARD_HEIGHT
			 || (board[_offset_y + y][_offset_x + x].r || board[_offset_y + y][_offset_x + x].g || board[_offset_y + y][_offset_x + x].b)
			 || _offset_x + x < 0
			 || _offset_x + x >= BOARD_WIDTH))
	      return 0;
	  }
      }

  for (x = 0; x < TETRIS_BRICK_SIZE; x++)
    {
      for (y = 0; y < TETRIS_BRICK_SIZE; y++)
	{
	  if (_offset_y + y < 0)
	    continue;

	  /* reverse */
	  else if (action == REVERSE && (brick[y][x].r || brick[y][x].g || brick[y][x].b)) {
	    board[_offset_y + y][_offset_x + x].r = 0;
	    board[_offset_y + y][_offset_x + x].g = 0;
	    board[_offset_y + y][_offset_x + x].b = 0;
}

	  /* NONE */
	  else if (action != REVERSE && (tmp[y][x].r || tmp[y][x].g || tmp[y][x].b))
	    board[_offset_y + y][_offset_x + x] = tmp[y][x];
	}
    }

  memcpy (brick, tmp, sizeof (brick));
  return 1;
}

void
fullLines (void)
{
  int16_t y, z;
  uint8_t x;
  struct cRGB blk[BOARD_HEIGHT][BOARD_WIDTH], dst[BOARD_HEIGHT][BOARD_WIDTH];

  z = BOARD_HEIGHT - 1;
  memset (blk, 0, sizeof (blk));
  memset (dst, 0, sizeof (dst));

  /* search full lines */
  for (y = BOARD_HEIGHT - 1; y >= 0; y--)
    for (x = 0; x < BOARD_WIDTH; x++)
      if (!board[y][x].r && !board[y][x].g && !board[y][x].b)
	{
	  memcpy (blk[y], board[y], sizeof (blk[y]));
	  memcpy (dst[z], board[y], sizeof (dst[z]));
	  z--;
	  break;
	}

  /* nothing found */
  if (z < 0)
    return;

  /* blink */
  for (z = 0; z < 3; z++)
    {
      output ();
      ms_sleep (tick);
      output ();
      ms_sleep (tick);
    }

  /* remove lines */
  memcpy (board, dst, sizeof (board));
}

uint8_t
nextStep (enum tetris_actions action)
{
  /* new brick */
  if (offset_y >= BOARD_HEIGHT)
    {
      uint8_t x, y;
      uint8_t rand_brick = rand () % (sizeof (bricks) / sizeof (bricks[0]));
      uint8_t rand_color = rand () % (sizeof (colors) / sizeof (colors[0]) - 1) + 1;

      offset_x = (BOARD_WIDTH - TETRIS_BRICK_SIZE) / 2;
      offset_y = -TETRIS_BRICK_SIZE + 1;

      memset (brick, 0, sizeof (brick));
      for (x = 0; x < TETRIS_BRICK_SIZE; x++)
	for (y = 0; y < TETRIS_BRICK_SIZE; y++)
	  if (bricks[rand_brick][x][y]) {
            brick[x][y].r = colors[rand_color].r;
            brick[x][y].g = colors[rand_color].g;
            brick[x][y].b = colors[rand_color].b;
      memset (brick, 0xFF, sizeof (brick));
}

      return insertBrick (offset_x, offset_y, NONE);
    }

  /* move brick */
  else
    {
      insertBrick (offset_x, offset_y, REVERSE);
      offset_y++;

      /* move left */
      if (action == MOVE_LEFT && insertBrick (offset_x - 1, offset_y, NONE))
	{
	  offset_x--;
	  return 1;
	}

      /* move right */
      else if (action == MOVE_RIGHT
	       && insertBrick (offset_x + 1, offset_y, NONE))
	{
	  offset_x++;
	  return 1;
	}

      /* rotate left */
      else if (action == ROTATE_LEFT
	       && insertBrick (offset_x, offset_y, ROTATE_LEFT))
	return 1;

      /* rotate right */
      else if (action == ROTATE_RIGHT
	       && insertBrick (offset_x, offset_y, ROTATE_RIGHT))
	return 1;

      /* move down */
      // TODO: move down

      /* nothing */
      else if (insertBrick (offset_x, offset_y, NONE))
	return 1;

      /* stay */
      else
	{
	  insertBrick (offset_x, offset_y - 1, NONE);
	  fullLines ();
	  offset_x = INT16_MAX;
	  offset_y = INT16_MAX;
	  return nextStep (0);
	}
    }

  return 0;
}

void
output ()
{
  uint16_t x;
  uint16_t y;

  struct cRGB leds[BOARD_WIDTH * BOARD_HEIGHT];
  memset(leds, 0, sizeof(leds));

  for (x = 0; x < BOARD_WIDTH; x++)
      for (y = 0; y < BOARD_HEIGHT; y++)
        {
    	leds[x * BOARD_HEIGHT + y].r = board[y][x].r;
  	leds[x * BOARD_HEIGHT + y].g = board[y][x].g;
	leds[x * BOARD_HEIGHT + y].b = board[y][x].b;
      }

  ws2812_setleds (leds, BOARD_WIDTH * BOARD_HEIGHT);
}

int
tetris_main (void)
{
  enum tetris_actions action;

colors[0].r = 0x00; colors[0].g = 0x00; colors[0].b = 0x00;
colors[1].r = 0xFF; colors[1].g = 0x00; colors[1].b = 0x00;
colors[2].r = 0x00; colors[2].g = 0xFF; colors[2].b = 0x00;
colors[3].r = 0x00; colors[3].g = 0x00; colors[3].b = 0xFF;
colors[4].r = 0xFF; colors[4].g = 0xFF; colors[4].b = 0x00;
colors[5].r = 0xFF; colors[5].g = 0x00; colors[5].b = 0xFF;
colors[6].r = 0x00; colors[6].g = 0xFF; colors[6].b = 0xFF;

  memset (board, 0xFF, sizeof (board));
  output();
  ms_sleep(tick);
  memset (board, 0, sizeof (board));

  while (1)
    {
      ms_sleep (tick);
      action = NONE;
      if (!nextStep (action))
	break;
      output ();
      ms_sleep (tick);
    }
  return 0;
}
