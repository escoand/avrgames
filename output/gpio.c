#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "gpio.h"

uint32_t        gpioColors[] = { GPIO_COLOR_MAPPING };
ws2811_t        ledstring = {
    .freq = BOARD_STRIP_FREQ,
    .dmanum = BOARD_STRIP_DMA,
    .channel = {
		[0] = {
		       .gpionum = BOARD_STRIP_GPIO,
		       .count = BOARD_WIDTH * BOARD_HEIGHT,
		       .invert = BOARD_STRIP_INVERT,
		       .brightness = BOARD_STRIP_BRIGHTNESS,
		       .strip_type = BOARD_STRIP_TYPE,
		       }
		}
    ,
};


uint32_t
getColor(uint32_t colorId)
{
    for (uint8_t i = 0; i < sizeof(gpioColors) / sizeof(uint32_t); i += 2) {
	if (gpioColors[i] == colorId)
	    return gpioColors[++i];
    }
    return GPIO_COLOR_NONE;
}


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

#if BOARD_STRIP_MODE == 0
	    pos = (x * BOARD_HEIGHT) + y;
#elif BOARD_STRIP_MODE == 1
	    pos = (x * BOARD_HEIGHT) + (BOARD_HEIGHT - y);
#elif BOARD_STRIP_MODE == 2
	    pos =
		(x * BOARD_HEIGHT) + (x % 2 ==
				      0 ? y : BOARD_HEIGHT - y - 1);
#elif BOARD_STRIP_MODE == 3
	    pos =
		(x * BOARD_HEIGHT) + (x % 2 ==
				      0 ? BOARD_HEIGHT - y - 1 : y);
#endif

#if BOARD_STRIP_REVERSE == 1
	    pos = BOARD_WIDTH * BOARD_HEIGHT - pos - 1;
#endif

	    ledstring.channel[0].leds[pos] = getColor((*board)[y][x]);
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

    for (uint8_t i = 0; i < sizeof(ledstring.channel[0].leds); i++) {
	ledstring.channel[0].leds[i] = getColor(BOARD_COLOR_NONE);
    }

    if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS) {
	fprintf(stderr, "ws2811_render failed: %s\n",
		ws2811_get_return_t_str(ret));
	return;
    }
}
