#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "io.h"

#ifdef __AVR__
#include <light_ws2812.h>
#else
#include <unistd.h>
#if _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#endif
#endif

uint32_t output_colors[] = {
#ifdef __AVR__
  0x000000, 0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0xFF00FF, 0x00FFFF
#else
  0x20, 0x23, 0x24, 0x26, 0x3d, 0x40, 0x4f
#endif
};

void
initOutput (void)
{
#ifdef __AVR__
#elif _WIN32
  DWORD mode;
  HANDLE hstdin;

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

void
output (uint8_t board[BOARD_HEIGHT][BOARD_WIDTH])
{
  uint16_t x;
  uint16_t y;

#ifdef __AVR__
  struct cRGB leds[BOARD_WIDTH * BOARD_HEIGHT];

  for (y = 0; y < BOARD_HEIGHT; y++)
    for (x = 0; x < BOARD_WIDTH; x++)
      {
	leds[x * BOARD_HEIGHT + y].r =
	  (output_colors[board[y][x]] & 0x00ff0000) >> 16;
	leds[x * BOARD_HEIGHT + y].g =
	  (output_colors[board[y][x]] & 0x0000ff00) >> 8;
	leds[x * BOARD_HEIGHT + y].b =
	  (output_colors[board[y][x]] & 0x000000ff) >> 0;
      }

  ws2812_setleds (leds, BOARD_WIDTH * BOARD_HEIGHT);
#else
#if _WIN32
  system ("cls");
#else
  system ("clear");
#endif
  printf ("+---------------------+\n");

  for (y = 0; y < BOARD_HEIGHT; y++)
    {
      printf ("| ");
      for (x = 0; x < BOARD_WIDTH; x++)
	{
	  printf ("%c ", output_colors[board[y][x]]);
	}
      printf ("|\n");
    }

  printf ("+---------------------+\n");
#endif
}

uint8_t
getKey (void)
{
  uint8_t key = 0;

#ifdef __AVR__
#else
  uint8_t buf[128];
  uint8_t len;

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
      key = buf[len - 1];
    }
#endif

  return key;
}
