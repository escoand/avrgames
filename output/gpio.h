#ifndef OUTPUT_GPIO_H_
#define OUTPUT_GPIO_H_

#include <ws2811.h>
#include "output.h"

/*
 * MODE:    - 0 -   - 1 -   - 2 -   - 3 -
 *          1 4 7   3 6 9   1 6 7   3 4 9
 *          2 5 8   2 5 8   2 5 8   2 5 8
 *          3 6 9   1 4 7   3 4 9   1 6 7
 * REVERSE: numbering backwards
 */

#define BOARD_STRIP_MODE        2
#define BOARD_STRIP_REVERSE     1

#define BOARD_STRIP_BRIGHTNESS  255
#define BOARD_STRIP_INVERT      0
#define BOARD_STRIP_FREQ        WS2811_TARGET_FREQ
#define BOARD_STRIP_GPIO        18
#define BOARD_STRIP_DMA         10
#define BOARD_STRIP_TYPE        WS2812_STRIP


#define GPIO_COLOR_FACTOR   0xBBBBBB

#define GPIO_COLOR_NONE     0x0
#define GPIO_COLOR_WHITE    0xFFFFFF & GPIO_COLOR_FACTOR
#define GPIO_COLOR_RED      0xFF0000 & GPIO_COLOR_FACTOR
#define GPIO_COLOR_GREEN    0x00FF00 & GPIO_COLOR_FACTOR
#define GPIO_COLOR_BLUE     0x0000FF & GPIO_COLOR_FACTOR
#define GPIO_COLOR_YELLOW   0xFFFF00 & GPIO_COLOR_FACTOR
#define GPIO_COLOR_PINK     0xFF00FF & GPIO_COLOR_FACTOR
#define GPIO_COLOR_CYAN     0x00FFFF & GPIO_COLOR_FACTOR
#define GPIO_COLOR_BROWN    0xF4A460

#define GPIO_COLOR_MAPPING  BOARD_COLOR_NONE,   GPIO_COLOR_NONE, \
                            BOARD_COLOR_WHITE,  GPIO_COLOR_WHITE, \
                            BOARD_COLOR_RED,    GPIO_COLOR_RED, \
                            BOARD_COLOR_GREEN,  GPIO_COLOR_GREEN, \
                            BOARD_COLOR_BLUE,   GPIO_COLOR_BLUE, \
                            BOARD_COLOR_YELLOW, GPIO_COLOR_YELLOW, \
                            BOARD_COLOR_PINK,   GPIO_COLOR_PINK, \
                            BOARD_COLOR_CYAN,   GPIO_COLOR_CYAN, \
                            BOARD_COLOR_BROWN,  GPIO_COLOR_BROWN

#endif				/* OUTPUT_GPIO_H_ */
