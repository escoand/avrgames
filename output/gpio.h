#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>

/*
 * MODE: - 0 - - 1 - - 2 - - 3 - 1 4 7 3 6 9 1 6 7 3 4 9 2 5 8 2 5 8 2 5 8 
 * 2 5 8 3 6 9 1 4 7 3 4 9 1 6 7 REVERSE: numbering backwards 
 */

#define BOARD_STRIPE_MODE    3
#define BOARD_STRIPE_REVERSE 1

#define TARGET_FREQ             WS2811_TARGET_FREQ
#define GPIO_PIN                18
#define DMA                     10
// #define STRIP_TYPE WS2811_STRIP_RGB // WS2812/SK6812RGB integrated
// chip+leds
#define STRIP_TYPE              WS2811_STRIP_GBR	// WS2812/SK6812RGB
							// integrated
							// chip+leds
// #define STRIP_TYPE SK6812_STRIP_RGBW // SK6812RGBW (NOT SK6812RGB)

uint32_t        output_colors[] = {
    0x000000, 0xff0000, 0x00ff00, 0x0000ff, 0xffff00, 0xff00ff, 0x00ffff
};

#endif				/* GPIO_H_ */
