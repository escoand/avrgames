#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <log.h>

#include "output.h"
#include "terminal.h"


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
    static uint8_t  defaultPalette[] = { TERMINAL_PALETTE_DEFAULT };
    static uint8_t  firePalette[] = { TERMINAL_PALETTE_FIRE };
    static float    defaultFactor =
	(sizeof(defaultPalette) / sizeof(uint32_t) - 1) / 256.0;
    static float    fireFactor =
	(sizeof(firePalette) / sizeof(uint32_t) - 1) / 256.0;

    switch (palette) {
    case BOARD_PALETTE_FIRE:
	return firePalette[(uint8_t)
			   (color * fireFactor)];
    default:
	return defaultPalette[(uint8_t)
			      (color * defaultFactor)];
    }
}


void
initOutput(void)
{
}

void
setOutput(board_matrix * board)
{
    return setOutputUsePalette(board, BOARD_PALETTE_DEFAULT);
}

void
setOutputUsePalette(board_matrix * board, enum BOARD_PALETTE palette)
{
    clearOutput();
    drawHorizontalBorder();

    for (uint8_t y = 0; y < BOARD_HEIGHT; y++) {
	putchar(TERMINAL_BORDER_VERTICAL);
	putchar(' ');
	for (uint8_t x = 0; x < BOARD_WIDTH; x++) {
	    putchar(getChar((*board)[y][x], palette));
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
