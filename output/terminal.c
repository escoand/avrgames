#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <log.h>

#include "output.h"
#include "terminal.h"


static uint32_t defaultPalette[] = { TERMINAL_PALETTE_DEFAULT };
static uint32_t firePalette[] = { TERMINAL_PALETTE_FIRE };
static uint8_t  defaultCount = sizeof(defaultPalette) / sizeof(uint32_t);
static uint8_t  fireCount = sizeof(firePalette) / sizeof(uint32_t);
static float    defaultFactor =
    (sizeof(defaultPalette) / sizeof(uint32_t) - 1) / 256.0;
static float    fireFactor =
    (sizeof(firePalette) / sizeof(uint32_t) - 1) / 256.0;

void
drawHorizontalBorder(void)
{
    putchar(TERMINAL_BORDER_EDGE);
    for (uint8_t i = 0; i <= BOARD_WIDTH * 2; i++)
	putchar(TERMINAL_BORDER_HORIZONTAL);
    putchar(TERMINAL_BORDER_EDGE);
    putchar('\n');
}

uint8_t
getChar(uint8_t color, enum BOARD_PALETTE palette)
{
    switch (palette) {
    case BOARD_PALETTE_FIRE:
	return firePalette[color < fireCount ? color : fireCount - 1];
    default:
	return defaultPalette[color <
			      defaultCount ? color : defaultCount - 1];
    }
}


void
initOutput(void)
{
    clearOutput();
}

uint8_t
mapToPalette(uint8_t color, enum BOARD_PALETTE palette)
{
    switch (palette) {
    case BOARD_PALETTE_FIRE:
	return color * fireFactor;
    default:
	return color * defaultFactor;
    }
}

void
setOutputUsePalette(board_matrix * board, enum BOARD_PALETTE palette)
{
    uint8_t         char_;

#if _WIN32
    clearOutput();
#else
    printf("\e[;H");
#endif
    drawHorizontalBorder();

    for (uint8_t y = 0; y < BOARD_HEIGHT; y++) {
	putchar(TERMINAL_BORDER_VERTICAL);
	putchar(' ');
	for (uint8_t x = 0; x < BOARD_WIDTH; x++) {
	    char_ = getChar((*board)[y][x], palette);
	    putchar(char_);
	    putchar(' ');
	}
	putchar(TERMINAL_BORDER_VERTICAL);
	putchar('\n');
    }

    drawHorizontalBorder();
}

void
setRawOutputUsePalette(board_matrix * board, enum BOARD_PALETTE palette)
{
    uint8_t         index;
    uint8_t         char_;

#if _WIN32
    clearOutput();
#else
    printf("\e[;H");
#endif
    drawHorizontalBorder();

    for (uint8_t y = 0; y < BOARD_HEIGHT; y++) {
	putchar(TERMINAL_BORDER_VERTICAL);
	putchar(' ');
	for (uint8_t x = 0; x < BOARD_WIDTH; x++) {
	    index = mapToPalette((*board)[y][x], palette);
	    char_ = getChar(index, palette);
	    putchar(char_);
	    putchar(' ');
	}
	putchar(TERMINAL_BORDER_VERTICAL);
	putchar('\n');
    }

    drawHorizontalBorder();
}

void
clearOutput(void)
{
#if _WIN32
    system("cls");
#else
    system("clear");
#endif
}
