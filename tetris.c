#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#define usleep(x) Sleep(x / 1000)
#elif __AVR__
#include <avr/io.h>
#include <light_ws2812.h>
#else
#include <string.h>
#include <termios.h>
#include <sys/time.h>
#include <unistd.h>
#endif

#include "tetris.h"
uint8_t board[TETRIS_BOARD_WIDTH][TETRIS_BOARD_HEIGHT];
uint8_t brick[TETRIS_BRICK_SIZE][TETRIS_BRICK_SIZE];
uint16_t tick = 200;

uint8_t
canBeAt (int16_t offset_x, int16_t offset_y)
{
  uint16_t x;
  uint16_t y;

  /* horizontal borders */
  for (y = 0; y < TETRIS_BRICK_SIZE; y++)
    if (offset_x < 0 && brick[-1 - offset_x][y]
	|| offset_x > TETRIS_BOARD_WIDTH - TETRIS_BRICK_SIZE + 1
	&& brick[TETRIS_BOARD_WIDTH - offset_x][y])
      return 0;

  /* vertical borders */
  for (x = 0; x < TETRIS_BRICK_SIZE; x++)
    if (offset_y > TETRIS_BOARD_HEIGHT - TETRIS_BRICK_SIZE
	&& brick[x][TETRIS_BOARD_HEIGHT - offset_y])
      return 0;

  /* former bricks */
  for (x = 0; x < TETRIS_BRICK_SIZE; x++)
    {
      if (offset_x + x < 0 || offset_x + x >= TETRIS_BOARD_WIDTH)
	continue;
      for (y = 0; y < TETRIS_BRICK_SIZE; y++)
	{
	  if (offset_y + y < 0)
	    continue;

	  else if (board[offset_x + x][offset_y + y] !=
		   TETRIS_BIT_EMPTY && brick[x][y])
	    return 0;
	}
    }

  return 1;
}

void
rotate (uint8_t clockwise)
{
  uint16_t x;
  uint16_t y;
  uint8_t tmp[TETRIS_BRICK_SIZE][TETRIS_BRICK_SIZE];

  for (x = 0; x < TETRIS_BRICK_SIZE; x++)
    for (y = 0; y < TETRIS_BRICK_SIZE; y++)
      {
	if (clockwise)
	  tmp[TETRIS_BRICK_SIZE - y - 1][x] = brick[x][y];

	else
	  tmp[y][TETRIS_BRICK_SIZE - x - 1] = brick[x][y];
      }

  memcpy (brick, tmp, sizeof (brick));
}

void
insert (int16_t offset_x, int16_t offset_y, uint8_t reverse)
{
  uint16_t x;
  uint16_t y;

  for (x = 0; x < TETRIS_BRICK_SIZE; x++)
    {
      if (offset_x + x < 0)
	continue;
      for (y = 0; y < TETRIS_BRICK_SIZE; y++)
	{
	  if (offset_y + y < 0)
	    continue;

	  else if (reverse && brick[x][y])
	    board[offset_x + x][offset_y + y] = TETRIS_BIT_EMPTY;

	  else if (!reverse && brick[x][y])
	    board[offset_x + x][offset_y + y] = brick[x][y];
	}
    }
}

void
initOutput ()
{
#ifdef _WIN32
  DWORD mode;
  HANDLE hstdin;
  DWORD res;

  hstdin = GetStdHandle (STD_INPUT_HANDLE);
  GetConsoleMode (hstdin, &mode);
  SetConsoleMode (hstdin, 0);
#elif __AVR__
  CLKPR = _BV (CLKPCE);
  CLKPR = 0;
#else
  struct termios mode;

  tcgetattr (STDIN_FILENO, &mode);
  mode.c_lflag &= ~(ICANON | ECHO);
  tcsetattr (STDIN_FILENO, TCSANOW, &mode);
#endif
}

void
output ()
{
  uint16_t x;
  uint16_t y;

#ifdef __AVR__
  uint8_t tmp[TETRIS_BOARD_WIDTH * TETRIS_BOARD_HEIGHT * 3];

  memset (tmp, 0, sizeof (tmp));
  for (x = 0; x < TETRIS_BOARD_WIDTH; x++)
    for (y = 0; y < TETRIS_BOARD_HEIGHT; y++)
      tmp[x][y * 3] = TETRIS_COLORS[board[x][y] - 1];
  ws2812_setleds (tmp, TETRIS_BOARD_WIDTH * TETRIS_BOARD_HEIGHT);
#else
  for (y = 0; y < TETRIS_BOARD_HEIGHT; y++)
    {
      printf ("| ");
      for (x = 0; x < TETRIS_BOARD_WIDTH; x++)
	{
	  printf ("%c ", TETRIS_COLORS[board[x][y] - 1]);
	}
      printf ("|\n");
    }
  printf (">---------------------<\n");
#endif
}

uint8_t
getkey ()
{
  uint8_t buf[128];
  uint8_t len;
  uint8_t ch = ' ';

#ifdef _WIN32
  if (kbhit ())
#else
  struct timeval tv;
  fd_set fds;

  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO (&fds);
  FD_SET (STDIN_FILENO, &fds);

  select (STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
  if (FD_ISSET (STDIN_FILENO, &fds))
#endif
    {
      len = read (STDIN_FILENO, buf, sizeof (buf) - 1);
      ch = buf[len - 1];
    }

  return ch;
}

int
main (void)
{
  uint16_t x;
  uint16_t y;
  int16_t offset_x;
  int16_t offset_y;
  uint8_t rand_brick;
  uint8_t rand_color;
  uint8_t key;

  memset (board, TETRIS_BIT_EMPTY, sizeof (board));
  initOutput ();

  while (1)
    {
      offset_x = (TETRIS_BOARD_WIDTH - TETRIS_BRICK_SIZE) / 2;
      offset_y = -TETRIS_BRICK_SIZE + 1;

      /* new brick */
      rand_brick =
	rand () % (sizeof (TETRIS_BRICKS) / sizeof (TETRIS_BRICKS[0]));
      rand_color =
	1 + rand () % (sizeof (TETRIS_COLORS) / sizeof (TETRIS_COLORS[0]));
      memset (brick, 0, sizeof (brick));
      for (x = 0; x < TETRIS_BRICK_SIZE; x++)
	for (y = 0; y < TETRIS_BRICK_SIZE; y++)
	  if (TETRIS_BRICKS[rand_brick][x][y])
	    brick[x][y] = rand_color;

      /* game over */
      if (!canBeAt (offset_x, offset_y))
	{
	  printf (" !!! GAME OVER !!!  ");
	  return EXIT_FAILURE;
	}

      for (; offset_y <= TETRIS_BOARD_HEIGHT; offset_y++)
	{
	  insert (offset_x, offset_y - 1, 1);

	  /* key actions */
	  key = getkey ();
	  if (key == 'a' && canBeAt (offset_x - 1, offset_y))
	    offset_x--;
	  else if (key == 'd' && canBeAt (offset_x + 1, offset_y))
	    offset_x++;
	  else if (key == 'e')
	    {
	      rotate (1);
	      if (!canBeAt (offset_x, offset_y))
		rotate (0);
	    }
	  else if (key == 'q')
	    {
	      rotate (0);
	      if (!canBeAt (offset_x, offset_y))
		rotate (1);
	    }

	  /* go down */
	  if (!canBeAt (offset_x, offset_y))
	    {
	      insert (offset_x, offset_y - 1, 0);
	      break;
	    }

	  insert (offset_x, offset_y, 0);
	  output ();
	  usleep (tick * 1000);
	}
    }

  return EXIT_SUCCESS;
}
