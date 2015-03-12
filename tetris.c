#include <stdlib.h>
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

#define TETRIS_BRICK_SIZE   4

uint8_t bricks[][TETRIS_BRICK_SIZE][TETRIS_BRICK_SIZE] = {
  {
   {0, 0, 0, 0}
   ,
   {0, 0, 0, 0}
   ,
   {1, 1, 1, 1}
   ,
   {0, 0, 0, 0}
   }
  ,
  {
   {0, 0, 0, 0}
   ,
   {1, 0, 0, 0}
   ,
   {1, 1, 1, 0}
   ,
   {0, 0, 0, 0}
   }
  ,
  {
   {0, 0, 0, 0}
   ,
   {0, 0, 1, 0}
   ,
   {1, 1, 1, 0}
   ,
   {0, 0, 0, 0}
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
   {0, 1, 1, 0}
   ,
   {1, 1, 0, 0}
   ,
   {0, 0, 0, 0}
   }
  ,
  {
   {0, 0, 0, 0}
   ,
   {1, 1, 0, 0}
   ,
   {0, 1, 1, 0}
   ,
   {0, 0, 0, 0}
   }
  ,
  {
   {0, 0, 0, 0}
   ,
   {0, 0, 1, 0}
   ,
   {0, 1, 1, 1}
   ,
   {0, 0, 0, 0}
   }
};

enum actions
{ INSERT, REVERSE, ROTATE_LEFT, ROTATE_RIGHT };

uint8_t brick[TETRIS_BRICK_SIZE][TETRIS_BRICK_SIZE];
int16_t offset_x = INT16_MAX;
int16_t offset_y = INT16_MAX;

uint8_t
insertBrick (uint8_t board[BOARD_WIDTH][BOARD_HEIGHT], int16_t _offset_x,
	     int16_t _offset_y, enum actions action)
{
  uint16_t x;
  uint16_t y;
  uint8_t tmp[TETRIS_BRICK_SIZE][TETRIS_BRICK_SIZE];

  /* action */
  if (action == ROTATE_LEFT || action == ROTATE_RIGHT)
    {
      for (x = 0; x < TETRIS_BRICK_SIZE; x++)
	for (y = 0; y < TETRIS_BRICK_SIZE; y++)
	  {
	    if (action == ROTATE_LEFT)
	      tmp[TETRIS_BRICK_SIZE - y - 1][x] = brick[x][y];

	    else if (action == ROTATE_RIGHT)
	      tmp[y][TETRIS_BRICK_SIZE - x - 1] = brick[x][y];
	  }
    }
  else
    memcpy (tmp, brick, sizeof (brick));

  /* check */
  if (action != REVERSE)
    for (x = 0; x < TETRIS_BRICK_SIZE; x++)
      {
	for (y = 0; y < TETRIS_BRICK_SIZE; y++)
	  {
	    if (_offset_y + y < 0)
	      continue;

	    else if (tmp[x][y]
		     && (_offset_y + y >= BOARD_HEIGHT
			 || board[_offset_x + x][_offset_y + y]
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
	  else if (action == REVERSE && brick[x][y])
	    board[_offset_x + x][_offset_y + y] = 0;

	  /* insert */
	  else if (action != REVERSE && tmp[x][y])
	    board[_offset_x + x][_offset_y + y] = tmp[x][y];
	}
    }

  memcpy (brick, tmp, sizeof (brick));
  return 1;
}

uint8_t
nextStep (uint8_t board[BOARD_WIDTH][BOARD_HEIGHT], uint8_t key)
{
  /* new brick */
  if (offset_y >= BOARD_HEIGHT)
    {
      uint16_t x, y;
      uint8_t rand_brick = rand () % (sizeof (bricks) / sizeof (bricks[0]));
      uint8_t rand_color = rand () % OUTPUT_COLORS_COUNT + 1;

      offset_x = (BOARD_WIDTH - TETRIS_BRICK_SIZE) / 2;
      offset_y = -TETRIS_BRICK_SIZE + 1;

      memset (brick, 0, sizeof (brick));
      for (x = 0; x < TETRIS_BRICK_SIZE; x++)
	for (y = 0; y < TETRIS_BRICK_SIZE; y++)
	  if (bricks[rand_brick][x][y])
	    brick[x][y] = rand_color;

      return insertBrick (board, offset_x, offset_y, INSERT);
    }

  /* move brick */
  else
    {
      insertBrick (board, offset_x, offset_y, REVERSE);
      offset_y++;

      /* move left */
      if (key == 'a' && insertBrick (board, offset_x - 1, offset_y, INSERT))
	{
	  offset_x--;
	  return 1;
	}

      /* move right */
      else if (key == 'd'
	       && insertBrick (board, offset_x + 1, offset_y, INSERT))
	{
	  offset_x++;
	  return 1;
	}

      /* rotate left */
      else if (key == 'e'
	       && insertBrick (board, offset_x, offset_y, ROTATE_LEFT))
	return 1;

      /* rotate right */
      else if (key == 'q'
	       && insertBrick (board, offset_x, offset_y, ROTATE_RIGHT))
	return 1;

      /* move down */
      else if (insertBrick (board, offset_x, offset_y, INSERT))
	return 1;

      /* stay */
      else
	{
	  insertBrick (board, offset_x, offset_y - 1, INSERT);
	  offset_x = INT16_MAX;
	  offset_y = INT16_MAX;
	  return nextStep (board, 0);
	}
    }

  return 0;
}

int tetris_main() {
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