#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "io.h"

#ifdef __AVR__
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#else
#include <unistd.h>
#if _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#endif
#endif


#ifdef __AVR__
#if BOARD_STRIPE_MODE == 0
#define _Y(i) (i % BOARD_HEIGHT)
#elif BOARD_STRIPE_MODE == 1
#define _Y(i) (BOARD_HEIGHT - (i % BOARD_HEIGHT) - 1)
#elif BOARD_STRIPE_MODE == 2
#define _Y(i) (abs ((i % 2 ? BOARD_HEIGHT - 1 : 0) - i % BOARD_HEIGHT))
#elif BOARD_STRIPE_MODE == 3
#define _Y(i) (abs ((i % 2 ? 0 : BOARD_HEIGHT - 1) - i % BOARD_HEIGHT))
#endif
#endif


/* ported from https://github.com/cpldcpu/light_ws2812 */
#ifdef __AVR__
// Timing in ns
#define w_zeropulse   350
#define w_onepulse    900
#define w_totalperiod 1250

// Fixed cycles used by the inner loop
#define w_fixedlow    2
#define w_fixedhigh   4
#define w_fixedtotal  18

// Insert NOPs to match the timing, if possible
#define w_zerocycles    (((F_CPU/1000)*w_zeropulse          )/1000000)
#define w_onecycles     (((F_CPU/1000)*w_onepulse    +500000)/1000000)
#define w_totalcycles   (((F_CPU/1000)*w_totalperiod +500000)/1000000)

// w1 - nops between rising edge and falling edge - low
#define w1 (w_zerocycles-w_fixedlow)
// w2   nops between fe low and fe high
#define w2 (w_onecycles-w_fixedhigh-w1)
// w3   nops to complete loop
#define w3 (w_totalcycles-w_fixedtotal-w1-w2)

#if w1>0
#define w1_nops w1
#else
#define w1_nops  0
#endif

// The only critical timing parameter is the minimum pulse length of the "0"
// Warn or throw error if this timing can not be met with current F_CPU settings.
#define w_lowtime ((w1_nops+w_fixedlow)*1000000)/(F_CPU/1000)
#if w_lowtime>550
#error "Light_ws2812: Sorry, the clock speed is too low. Did you set F_CPU correctly?"
#elif w_lowtime>450
#warning "Light_ws2812: The timing is critical and may only work on WS2812B, not on WS2812(S)."
#warning "Please consider a higher clockspeed, if possible"
#endif

#if w2>0
#define w2_nops w2
#else
#define w2_nops  0
#endif

#if w3>0
#define w3_nops w3
#else
#define w3_nops  0
#endif

#define w_nop1  "nop      \n\t"
#define w_nop2  "rjmp .+0 \n\t"
#define w_nop4  w_nop2 w_nop2
#define w_nop8  w_nop4 w_nop4
#define w_nop16 w_nop8 w_nop8
#endif


#ifdef __AVR__
uint32_t output_colors[] = {
  0x000000, 0xff0000, 0x00ff00, 0x0000ff, 0xffff00, 0xff00ff, 0x00ffff
};

uint8_t maskhi, masklo;
#else
uint32_t output_colors[] = {
  0x20, 0x23, 0x24, 0x26, 0x3d, 0x40, 0x4f
};
#endif

void
initOutput (void)
{
#ifdef __AVR__

  /* set direction */
  DDRREG_STRIPE |= _BV (PINBIT_STRIPE);
  DDRREG_LEFT &= ~(_BV (PINBIT_LEFT));
  DDRREG_RIGHT &= ~(_BV (PINBIT_RIGHT));
  DDRREG_UP &= ~(_BV (PINBIT_UP));
  DDRREG_DOWN &= ~(_BV (PINBIT_DOWN));

  /* activate pull-ups */
  PORTREG_LEFT |= _BV (PINBIT_LEFT);
  PORTREG_RIGHT |= _BV (PINBIT_RIGHT);
  PORTREG_UP |= _BV (PINBIT_UP);
  PORTREG_DOWN |= _BV (PINBIT_DOWN);

  maskhi = _BV (PINBIT_STRIPE);
  masklo = ~maskhi & PORTREG_STRIPE;
  maskhi |= PORTREG_STRIPE;
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

/* ported from https://github.com/cpldcpu/light_ws2812 */
#ifdef __AVR__
void
ws2818_bytes (board_matrix * board)
{
  uint8_t ctr, cur;

  // TODO: better skip mechanism
  for (int16_t i = BOARD_HEIGHT * BOARD_WIDTH - 1; i >= 0; i--)
    {
      switch(i % 3) {
        case 0: cur = (output_colors[(*board)[_Y(i)][int (i / BOARD_HEIGHT)]] & 0x00ff00) >> 8;
        case 1: cur = (output_colors[(*board)[_Y(i)][int (i / BOARD_HEIGHT)]] & 0xff0000) >> 16;
        case 2: cur = (output_colors[(*board)[_Y(i)][int (i / BOARD_HEIGHT)]] & 0x0000ff) >> 0;
      }

      asm volatile ("       ldi   %0,8  \n\t" "loop%=:            \n\t" "       out   %2,%3 \n\t"	//  '1' [01] '0' [01] - re
#if (w1_nops&1)
		    w_nop1
#endif
#if (w1_nops&2)
		    w_nop2
#endif
#if (w1_nops&4)
		    w_nop4
#endif
#if (w1_nops&8)
		    w_nop8
#endif
#if (w1_nops&16)
		    w_nop16
#endif
		    "       sbrs  %1,7  \n\t"	//  '1' [03] '0' [02]
		    "       out   %2,%4 \n\t"	//  '1' [--] '0' [03] - fe-low
		    "       lsl   %1    \n\t"	//  '1' [04] '0' [04]
#if (w2_nops&1)
		    w_nop1
#endif
#if (w2_nops&2)
		    w_nop2
#endif
#if (w2_nops&4)
		    w_nop4
#endif
#if (w2_nops&8)
		    w_nop8
#endif
#if (w2_nops&16)
		    w_nop16
#endif
		    "       out   %2,%4 \n\t"	//  '1' [+1] '0' [+1] - fe-high
#if (w3_nops&1)
		    w_nop1
#endif
#if (w3_nops&2)
		    w_nop2
#endif
#if (w3_nops&4)
		    w_nop4
#endif
#if (w3_nops&8)
		    w_nop8
#endif
#if (w3_nops&16)
		    w_nop16
#endif
		    "       dec   %0    \n\t"	//  '1' [+2] '0' [+2]
		    "       brne  loop%=\n\t"	//  '1' [+3] '0' [+4]
		    :"=&d" (ctr):"r" (cur),
		    "I" (_SFR_IO_ADDR (PORTREG_STRIPE)), "r" (maskhi),
		    "r" (masklo));
    }
}
#endif

void
output (board_matrix * board)
{
#ifdef __AVR__
  uint16_t prev;

  prev = SREG;
  cli ();

  ws2818_bytes (&board);

  SREG = prev;
  _delay_us (50);

  sei ();
#else
  uint16_t x, y;

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
	  printf ("%c ", output_colors[(*board)[y][x]]);
	}
      printf ("|\n");
    }

  printf ("+---------------------+\n");
#endif
}

uint8_t
getButton (void)
{
  uint8_t button = 0;

#ifdef __AVR__
  if (bit_is_clear (PINREG_LEFT, PINBIT_LEFT))
    button = BUTTON_LEFT;
  else if (bit_is_clear (PINREG_RIGHT, PINBIT_RIGHT))
    button = BUTTON_RIGHT;
  else if (bit_is_clear (PINREG_UP, PINBIT_UP))
    button = BUTTON_UP;
  else if (bit_is_clear (PINREG_DOWN, PINBIT_DOWN))
    button = BUTTON_DOWN;
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
      button = buf[len - 1];
    }
#endif

  return button;
}
