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


#define GPIO_PALETTE_DEFAULT \
                             0xFFFFFF, 0xFF0000, 0x00FF00, 0x0000FF, \
                             0xFFFF00, 0xFF00FF, 0x00FFFF

#define GPIO_PALETTE_RAINBOW \
                             0xFF0000, 0xD52A00, 0xAB5500, 0xAB7F00, \
                             0xABAB00, 0x56D500, 0x00FF00, 0x00D52A, \
                             0x00AB55, 0x0056AA, 0x0000FF, 0x2A00D5, \
                             0x5500AB, 0x7F0081, 0xAB0055, 0xD5002B

#define GPIO_PALETTE_FIRE    0x000000, \
                             0x330000, 0x660000, 0x990000, 0xCC0000, 0xFF0000, \
                             0xFF3300, 0xFF6600, 0xFF9900, 0xFFCC00, 0xFFFF00, \
                             0xFFFF33, 0xFFFF66, 0xFFFF99, 0xFFFFCC, 0xFFFFFF

#endif				/* OUTPUT_GPIO_H_ */
