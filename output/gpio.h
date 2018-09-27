#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>
#include <ws2811.h>

/*
 * MODE:    - 0 -   - 1 -   - 2 -   - 3 -
 *          1 4 7   3 6 9   1 6 7   3 4 9
 *          2 5 8   2 5 8   2 5 8   2 5 8
 *          3 6 9   1 4 7   3 4 9   1 6 7
 * REVERSE: numbering backwards
 */

#define BOARD_STRIP_MODE        3
#define BOARD_STRIP_REVERSE     1
#define BOARD_STRIP_BRIGHTNESS  128
#define BOARD_STRIP_INVERT      0
#define BOARD_STRIP_FREQ        WS2811_TARGET_FREQ
#define BOARD_STRIP_GPIO        18
#define BOARD_STRIP_DMA         10
#define BOARD_STRIP_TYPE        WS2811_STRIP_GBR

uint32_t        output_colors[] = {
    0x000000, 0xff0000, 0x00ff00, 0x0000ff, 0xffff00, 0xff00ff, 0x00ffff
};

#endif				/* GPIO_H_ */
