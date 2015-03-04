#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#define usleep(x) Sleep(x / 1000)
#elif __AVR__
#else
#include <string.h>
#include <termios.h>
#include <sys/time.h>
#include <unistd.h>
#endif

#include "tetris.h"
unsigned long board[TETRIS_BOARD_WIDTH][TETRIS_BOARD_HEIGHT];
unsigned long brick[TETRIS_BRICK_SIZE][TETRIS_BRICK_SIZE];
unsigned int tick = 200;

char
canBeAt (signed short offset_x, signed short offset_y)
{
  unsigned short x;
  unsigned short y;

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
rotate (char clockwise)
{
  unsigned short x;
  unsigned short y;
  unsigned long tmp[TETRIS_BRICK_SIZE][TETRIS_BRICK_SIZE];

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
insert (signed short offset_x, signed short offset_y, char reverse)
{
  unsigned short x;
  unsigned short y;

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
output ()
{
  unsigned short x;
  unsigned short y;

  for (y = 0; y < TETRIS_BOARD_HEIGHT; y++)
    {
      printf ("| ");
      for (x = 0; x < TETRIS_BOARD_WIDTH; x++)
	{
	  printf ("%c ", board[x][y]);
	}
      printf ("|\n");
    }

  printf (">---------------------<\n");
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
#else
  struct termios mode;

  tcgetattr (STDIN_FILENO, &mode);
  mode.c_lflag &= ~(ICANON | ECHO);
  tcsetattr (STDIN_FILENO, TCSANOW, &mode);
#endif
}

int
getkey ()
{
  unsigned char buf[128];
  unsigned char len;
  unsigned char ch = ' ';

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
  unsigned short x;
  unsigned short y;
  signed short offset_x;
  signed short offset_y;
  unsigned char rand_brick;
  unsigned char rand_output;
  unsigned char key;

  memset (board, TETRIS_BIT_EMPTY, sizeof (board));
  initOutput ();

  while (1)
    {
      offset_x = (TETRIS_BOARD_WIDTH - TETRIS_BRICK_SIZE) / 2;
      offset_y = -TETRIS_BRICK_SIZE + 1;

      /* new brick */
      rand_brick = rand () % (sizeof (TETRIS_BIT_OUTPUT) / sizeof (long));
      rand_output = rand () % (sizeof (TETRIS_BIT_OUTPUT) / sizeof (long));
      memset (brick, 0, sizeof (brick));
      for (x = 0; x < TETRIS_BRICK_SIZE; x++)
	for (y = 0; y < TETRIS_BRICK_SIZE; y++)
	  if (TETRIS_BRICKS[rand_brick][x][y])
	    brick[x][y] = TETRIS_BIT_OUTPUT[rand_output];

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
