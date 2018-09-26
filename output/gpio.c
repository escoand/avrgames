#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "output.h"
#include "gpio.h"
#include "../rpi_ws281x/ws2811.h"

ws2811_t        ledstring = {
    .freq = TARGET_FREQ,
    .dmanum = DMA,
    .channel = {
		[0] = {
		       .gpionum = GPIO_PIN,
		       .count = BOARD_WIDTH * BOARD_HEIGHT,
		       .invert = 0,
		       .brightness = 255,
		       .strip_type = STRIP_TYPE,
		       }
		}
    ,
};

void
initOutput(void)
{
    ws2811_return_t ret;

    if ((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS) {
	fprintf(stderr, "ws2811_init failed: %s\n",
		ws2811_get_return_t_str(ret));
	return;
    }
}

void
setOutput(board_matrix * board)
{
    uint16_t        pos;
    ws2811_return_t ret;

    for (uint8_t x = 0; x < BOARD_WIDTH; x++) {
	for (uint8_t y = 0; y < BOARD_HEIGHT; y++) {

#if BOARD_STRIPE_MODE == 0
	    pos = (x * BOARD_HEIGHT) + y;
#elif BOARD_STRIPE_MODE == 1
	    pos = (x * BOARD_HEIGHT) + (BOARD_HEIGHT - y);
#elif BOARD_STRIPE_MODE == 2
	    pos =
		(x * BOARD_HEIGHT) + (x % 2 ==
				      0 ? BOARD_HEIGHT - y - 1 : y);
#elif BOARD_STRIPE_MODE == 3
	    pos =
		(x * BOARD_HEIGHT) + (x % 2 ==
				      0 ? y : BOARD_HEIGHT - y - 1);
#endif

#if BOARD_STRIPE_REVERSE == 1
	    pos = (BOARD_WIDTH * BOARD_HEIGHT) - pos - 1;
#endif

	    ledstring.channel[0].leds[pos] = output_colors[(*board)[y][x]];
	}
    }

    if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS) {
	fprintf(stderr, "ws2811_render failed: %s\n",
		ws2811_get_return_t_str(ret));
	return;
    }
}

void
clearOutput(void)
{
    ws2811_return_t ret;

    for (uint8_t i = 0; i < BOARD_WIDTH * BOARD_HEIGHT; i++) {
	ledstring.channel[0].leds[i] = output_colors[0];
    }

    if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS) {
	fprintf(stderr, "ws2811_render failed: %s\n",
		ws2811_get_return_t_str(ret));
	return;
    }
}
