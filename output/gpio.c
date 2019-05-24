#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <log.h>

#include "gpio.h"


static uint32_t defaultPalette[] = { GPIO_PALETTE_DEFAULT };
static uint32_t rainbowPalette[] = { GPIO_PALETTE_RAINBOW };
static uint32_t firePalette[] = { GPIO_PALETTE_FIRE };
static uint8_t  defaultCount = sizeof(defaultPalette) / sizeof(uint32_t);
static uint8_t  rainbowCount = sizeof(rainbowPalette) / sizeof(uint32_t);
static uint8_t  fireCount = sizeof(firePalette) / sizeof(uint32_t);
static float    defaultFactor =
    (sizeof(defaultPalette) / sizeof(uint32_t) - 1) / 256.0;
static float    rainbowFactor =
    (sizeof(rainbowPalette) / sizeof(uint32_t) - 1) / 256.0;
static float    fireFactor =
    (sizeof(firePalette) / sizeof(uint32_t) - 1) / 256.0;

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
};


uint16_t
getRealPosition(uint8_t x, uint8_t y)
{
    uint16_t        pos;

#if BOARD_STRIP_MODE == 0
    pos = (x * BOARD_HEIGHT) + y;
#elif BOARD_STRIP_MODE == 1
    pos = (x * BOARD_HEIGHT) + (BOARD_HEIGHT - y);
#elif BOARD_STRIP_MODE == 2
    pos = (x * BOARD_HEIGHT) + (x % 2 == 0 ? y : BOARD_HEIGHT - y - 1);
#elif BOARD_STRIP_MODE == 3
    pos = (x * BOARD_HEIGHT) + (x % 2 == 0 ? BOARD_HEIGHT - y - 1 : y);
#endif

#if BOARD_STRIP_REVERSE == 1
    pos = BOARD_WIDTH * BOARD_HEIGHT - pos - 1;
#endif

    return pos;
}

uint32_t
getColor(uint8_t color, enum BOARD_PALETTE palette)
{
    switch (palette) {
    case BOARD_PALETTE_RAINBOW:
	return rainbowPalette[color <
			      rainbowCount ? color : rainbowCount - 1];
    case BOARD_PALETTE_FIRE:
	return firePalette[color < fireCount ? color : fireCount - 1];
    default:
	return defaultPalette[color <
			      defaultCount ? color : defaultCount - 1];
    }
}

void
renderOutput()
{
    ws2811_return_t ret;

    if ((ret = ws2811_render(&ledstring)) != WS2811_SUCCESS) {
	log_error("ws2811_render failed: %s",
		  ws2811_get_return_t_str(ret));
    }
}


void
initOutput(void)
{
    ws2811_return_t ret;

    if ((ret = ws2811_init(&ledstring)) != WS2811_SUCCESS) {
	log_error("ws2811_init failed: %s", ws2811_get_return_t_str(ret));
    }
}

uint8_t
mapToPalette(uint8_t color, enum BOARD_PALETTE palette)
{
    switch (palette) {
    case BOARD_PALETTE_RAINBOW:
	return color * rainbowFactor;
    case BOARD_PALETTE_FIRE:
	return color * fireFactor;
    default:
	return color * defaultFactor;
    }
}

void
setOutputUsePalette(board_matrix * board, enum BOARD_PALETTE palette)
{
    uint16_t        pos;
    uint8_t         index;
    uint32_t        color;

    for (uint8_t x = 0; x < BOARD_WIDTH; x++) {
	for (uint8_t y = 0; y < BOARD_HEIGHT; y++) {
	    pos = getRealPosition(x, y);
	    index = mapToPalette((*board)[y][x], palette);
	    color = getColor(index, palette);
	    ledstring.channel[0].leds[pos] = color;
	}
    }

    renderOutput();
}

void
setRawOutputUsePalette(board_matrix * board, enum BOARD_PALETTE palette)
{
    uint16_t        pos;
    uint32_t        color;

    for (uint8_t x = 0; x < BOARD_WIDTH; x++) {
	for (uint8_t y = 0; y < BOARD_HEIGHT; y++) {
	    pos = getRealPosition(x, y);
	    color = getColor((*board)[y][x], palette);
	    ledstring.channel[0].leds[pos] = color;
	}
    }

    renderOutput();
}

void
clearOutput(void)
{
    memset(ledstring.channel[0].leds, 0, ledstring.channel[0].count);
    renderOutput();
}
