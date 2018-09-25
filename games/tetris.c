#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../input/input.h"
#include "../output/output.h"
#include "tetris.h"

#ifdef __AVR__
#include <avr/io.h>
#include <util/delay.h>
#define ms_sleep(x) {uint16_t ms; for(ms=x;ms>0;ms--) _delay_ms(1);}
#elif _WIN32
#include <windows.h>
#define ms_sleep(x) Sleep(x)
#else
#include <unistd.h>
#define ms_sleep(x) usleep(x * 1000)
#endif

#define TETRIS_BRICK_SIZE   4

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

enum tetris_actions
{ ACTION_NONE, ACTION_MOVE_DOWN, ACTION_MOVE_LEFT, ACTION_MOVE_RIGHT,
  ACTION_ROTATE_LEFT, ACTION_ROTATE_RIGHT,
  ACTION_REVERSE
};

board_matrix board;
uint8_t brick[TETRIS_BRICK_SIZE][TETRIS_BRICK_SIZE];
int16_t offset_x = INT16_MAX;
int16_t offset_y = INT16_MAX;

uint16_t tick = 300;

uint8_t
insertBrick (int16_t _offset_x, int16_t _offset_y, enum tetris_actions action)
{
  uint8_t x, y, tmp[TETRIS_BRICK_SIZE][TETRIS_BRICK_SIZE];

  /* action */
  if (action == ACTION_ROTATE_LEFT || action == ACTION_ROTATE_RIGHT)
    {
      for (x = 0; x < TETRIS_BRICK_SIZE; x++)
	for (y = 0; y < TETRIS_BRICK_SIZE; y++)
	  {
	    if (action == ACTION_ROTATE_RIGHT)
	      tmp[x][TETRIS_BRICK_SIZE - y - 1] = brick[y][x];

	    else if (action == ACTION_ROTATE_LEFT)
	      tmp[TETRIS_BRICK_SIZE - x - 1][y] = brick[y][x];
	  }
    }
  else
    memcpy (tmp, brick, sizeof (tmp));

  /* check */
  if (action != ACTION_REVERSE)
    for (x = 0; x < TETRIS_BRICK_SIZE; x++)
      {
	for (y = 0; y < TETRIS_BRICK_SIZE; y++)
	  {
	    if (_offset_y + y < 0)
	      continue;

	    else if (tmp[y][x]
		     && (_offset_y + y >= BOARD_HEIGHT
			 || board[_offset_y + y][_offset_x + x]
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
	  else if (action == ACTION_REVERSE && brick[y][x])
	    board[_offset_y + y][_offset_x + x] = 0;

	  /* NONE */
	  else if (action != ACTION_REVERSE && tmp[y][x])
	    board[_offset_y + y][_offset_x + x] = tmp[y][x];
	}
    }

  memcpy (brick, tmp, sizeof (brick));
  return 1;
}

void
fullLines (void)
{
  uint8_t x, tmp[BOARD_HEIGHT][BOARD_WIDTH];
  int16_t y;
  uint8_t a, b;

  /* copy non full lines */
  memcpy (tmp, board, sizeof (board));
  b = 0;
  for (y = 0; y < BOARD_HEIGHT; y++)
  {
    a = UINT8_MAX;
    for (x = 0; x < BOARD_WIDTH; x++)
      a = a && board[y][x];
    if(a != 0) {
      memset(tmp[y], 0, sizeof(tmp[y]));
      b++;
    }
  }

  if (b == 0)
    return;

  /* blink */
  for (a = 0; a < 3; a++)
    {
      setOutput (&tmp);
      ms_sleep (tick);
      setOutput (&board);
      ms_sleep (tick);
    }

  /* remove full lines */
  memset (board, 0, sizeof (board));
  b = BOARD_HEIGHT - 1;
  for (y = BOARD_HEIGHT - 1; y >= 0; y--)
  {
    a = 0;
    for (x = 0; x < BOARD_WIDTH; x++)
      a = a || tmp[y][x];
    if(a != 0)
      memcpy(board[b--], tmp[y], sizeof(board[y]));
  }
}

uint8_t
nextStep (void)
{
  /* new brick */
  if (offset_y >= BOARD_HEIGHT)
    {
      uint8_t x, y;
      uint8_t rand_brick = rand () % (sizeof (bricks) / sizeof (bricks[0]));
      uint8_t rand_color = rand () % OUTPUT_COLORS_COUNT + 1;

      offset_x = (BOARD_WIDTH - TETRIS_BRICK_SIZE) / 2;
      offset_y = -TETRIS_BRICK_SIZE + 1;

      memset (brick, 0, sizeof (brick));
      for (x = 0; x < TETRIS_BRICK_SIZE; x++)
	for (y = 0; y < TETRIS_BRICK_SIZE; y++)
	  if (bricks[rand_brick][x][y])
	    brick[x][y] = rand_color;

      return insertBrick (offset_x, offset_y, ACTION_NONE);
    }

  /* move brick */
  else
    {
      uint8_t button = getInput ();

      insertBrick (offset_x, offset_y, ACTION_REVERSE);
      offset_y++;

      /* move left */
      if (button & BUTTON_LEFT
	  && insertBrick (offset_x - 1, offset_y, ACTION_NONE))
	{
	  offset_x--;
	  return 1;
	}

      /* move right */
      else if (button & BUTTON_RIGHT
	       && insertBrick (offset_x + 1, offset_y, ACTION_NONE))
	{
	  offset_x++;
	  return 1;
	}

      /* rotate left */
      else if (button & BUTTON_UP
	       && insertBrick (offset_x, offset_y, ACTION_ROTATE_LEFT))
	return 1;

      /* rotate right */
      else if (button & BUTTON_DOWN
	       && insertBrick (offset_x, offset_y, ACTION_ROTATE_RIGHT))
	return 1;

      /* move down */
      // TODO: move down

      /* nothing */
      else if (insertBrick (offset_x, offset_y, ACTION_NONE))
	return 1;

      /* stay */
      else
	{
	  insertBrick (offset_x, offset_y - 1, ACTION_NONE);
	  fullLines ();
	  offset_x = INT16_MAX;
	  offset_y = INT16_MAX;
	  return nextStep ();
	}
    }

  return 0;
}

int
tetris_main (void)
{
#ifdef __AVR__
  uint8_t seed = 0;
  uint8_t *p = (uint8_t*) (RAMEND+1);
  extern uint8_t __heap_start;
    
  while (p >= &__heap_start + 1)
    seed ^= * (--p);

  srand(seed);
#else
  srand((unsigned)time(NULL));
#endif

  memset (board, 0, sizeof (board));

  initInput ();
  initOutput ();

  while (1)
    {
      if (!nextStep ())
	return 1;

      setOutput (&board);
      ms_sleep (tick);
    }

  clearOutput ();

  return 0;
}
