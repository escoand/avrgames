#include <stdlib.h>
#include <string.h>
#include "io.h"
#include "tetris.h"

#ifdef __AVR__
#include <avr/io.h>
#include <util/delay.h>
#define ms_sleep(x) {uint16_t ms; for(ms=x;ms>0;ms--) _delay_ms(1);}
#elif _WIN32
#include <windows.h>
#define ms_sleep(x) Sleep(x)
#else
#include <sys/time.h>
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

uint8_t board[BOARD_HEIGHT][BOARD_WIDTH];
uint8_t brick[TETRIS_BRICK_SIZE][TETRIS_BRICK_SIZE];
int16_t offset_x = INT16_MAX;
int16_t offset_y = INT16_MAX;

uint16_t tick = 200;

uint8_t
insertBrick (int16_t _offset_x, int16_t _offset_y, enum tetris_actions action)
{
  uint8_t x, y, tmp[TETRIS_BRICK_SIZE][TETRIS_BRICK_SIZE];

  /* action */
  if (action == ROTATE_LEFT || action == ROTATE_RIGHT)
    {
      for (x = 0; x < TETRIS_BRICK_SIZE; x++)
	for (y = 0; y < TETRIS_BRICK_SIZE; y++)
	  {
	    if (action == ROTATE_RIGHT)
	      tmp[x][TETRIS_BRICK_SIZE - y - 1] = brick[y][x];

	    else if (action == ROTATE_LEFT)
	      tmp[TETRIS_BRICK_SIZE - x - 1][y] = brick[y][x];
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
	  else if (action == REVERSE && brick[y][x])
	    board[_offset_y + y][_offset_x + x] = 0;

	  /* NONE */
	  else if (action != REVERSE && tmp[y][x])
	    board[_offset_y + y][_offset_x + x] = tmp[y][x];
	}
    }

  memcpy (brick, tmp, sizeof (brick));
  return 1;
}

void
fullLines ()
{
  int16_t y, z;
  uint8_t x, blk[BOARD_HEIGHT][BOARD_WIDTH], dst[BOARD_HEIGHT][BOARD_WIDTH];

  z = BOARD_HEIGHT - 1;
  memset (blk, 0, sizeof (blk));
  memset (dst, 0, sizeof (dst));

  /* search full lines */
  for (y = BOARD_HEIGHT - 1; y >= 0; y--)
    for (x = 0; x < BOARD_WIDTH; x++)
      if (board[y][x] == 0)
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
      output (board);
      ms_sleep (tick);
      output (blk);
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
      uint8_t rand_color = rand () % OUTPUT_COLORS_COUNT + 1;
      // TODO: real random for windows

      offset_x = (BOARD_WIDTH - TETRIS_BRICK_SIZE) / 2;
      offset_y = -TETRIS_BRICK_SIZE + 1;

      memset (brick, 0, sizeof (brick));
      for (x = 0; x < TETRIS_BRICK_SIZE; x++)
	for (y = 0; y < TETRIS_BRICK_SIZE; y++)
	  if (bricks[rand_brick][x][y])
	    brick[x][y] = rand_color;

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

int
tetris_main ()
{
  enum tetris_actions action;
  memset (board, 0, sizeof (board));

#ifdef __AVR__
  DDRD |= _BV (PD7);
  DDRB |= _BV (PB0);
  PORTB |= _BV (PB0);
  PORTD |= _BV (PD7);
#endif

  initOutput ();

#ifdef __AVR__
  PORTB &= ~_BV (PB0);
  PORTD &= ~_BV (PD7);
#endif

  while (1)
    {
#ifdef __AVR__
      PORTB |= _BV (PB0);
      ms_sleep (500);
      action = NONE;
#else
      switch (getKey ())
	{
	case 's':
	  action = MOVE_DOWN;
	  break;
	case 'a':
	  action = MOVE_LEFT;
	  break;
	case 'd':
	  action = MOVE_RIGHT;
	  break;
	case 'q':
	  action = ROTATE_LEFT;
	  break;
	case 'e':
	  action = ROTATE_RIGHT;
	  break;
	default:
	  action = NONE;
	  break;
	}
#endif

      if (!nextStep (action))
	return 1;
#ifdef __AVR__
      PORTB &= ~_BV (PB0);
      PORTD |= _BV (PD7);
#endif
      output (board);
#ifdef __AVR__
      PORTD &= ~_BV (PD7);
#endif
      ms_sleep (tick);
    }

  return 0;
}
