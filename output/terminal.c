#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "output.h"
#include "terminal.h"

uint8_t         terminalChars[] = { TERMINAL_CHAR_MAPPING };


void
drawHorizontalBorder(void)
{
    putchar(TERMINAL_BORDER_EDGE);
    for (uint8_t i = 0; i <= BOARD_WIDTH * 2; i++)
	putchar(TERMINAL_BORDER_HORIZONTAL);
    putchar(TERMINAL_BORDER_EDGE);
    putchar('\n');
}

uint32_t
getChar(uint32_t colorId)
{
    for (uint8_t i = 0; i < sizeof(terminalChars) / sizeof(uint8_t);
	 i += 2) {
	if (terminalChars[i] == colorId)
	    return terminalChars[++i];
    }
    return TERMINAL_CHAR_NONE;
}


void
initOutput(void)
{
}

void
setOutput(board_matrix * board)
{
    clearOutput();
    drawHorizontalBorder();

    for (uint8_t y = 0; y < BOARD_HEIGHT; y++) {
	putchar(TERMINAL_BORDER_VERTICAL);
	putchar(' ');
	for (uint8_t x = 0; x < BOARD_WIDTH; x++) {
	    putchar(getChar((*board)[y][x]));
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
