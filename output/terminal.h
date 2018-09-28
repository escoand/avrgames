#ifndef OUTPUT_TERMINAL_H_
#define OUTPUT_TERMINAL_H_

#include "output.h"

#define TERMINAL_BORDER_VERTICAL   '|'
#define TERMINAL_BORDER_HORIZONTAL '-'
#define TERMINAL_BORDER_EDGE       '+'

#define TERMINAL_CHAR_NONE     ' '
#define TERMINAL_CHAR_WHITE    '#'
#define TERMINAL_CHAR_RED      '$'
#define TERMINAL_CHAR_GREEN    '&'
#define TERMINAL_CHAR_BLUE     '='
#define TERMINAL_CHAR_YELLOW   '@'
#define TERMINAL_CHAR_PINK     '%'
#define TERMINAL_CHAR_CYAN     '?'

#define TERMINAL_CHAR_MAPPING  BOARD_COLOR_NONE,   TERMINAL_CHAR_NONE, \
                               BOARD_COLOR_WHITE,  TERMINAL_CHAR_WHITE, \
                               BOARD_COLOR_RED,    TERMINAL_CHAR_RED, \
                               BOARD_COLOR_GREEN,  TERMINAL_CHAR_GREEN, \
                               BOARD_COLOR_BLUE,   TERMINAL_CHAR_BLUE, \
                               BOARD_COLOR_YELLOW, TERMINAL_CHAR_YELLOW, \
                               BOARD_COLOR_PINK,   TERMINAL_CHAR_PINK, \
                               BOARD_COLOR_CYAN,   TERMINAL_CHAR_CYAN

#endif				/* OUTPUT_TERMINAL_H_ */
