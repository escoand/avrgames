#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <inttypes.h>

#define DDR_LEDS            DDRB
#define PORT_LEDS           PORTB
#define PIN_LEDS            2

#define BOARD_WIDTH         10
#define BOARD_HEIGHT        18
#define BOARD_STRIPE_MODE   3	// 0=top-bottom 1=bottom-top 2=chain-from-top 3=chain-from-bottom
#define BOARD_OUTPUT_BLOCK  (2*BOARD_HEIGHT)

#define OUTPUT_COLORS_COUNT 6

typedef uint8_t board_matrix[BOARD_HEIGHT][BOARD_WIDTH];

void initOutput (void);
void output (board_matrix * board);
uint8_t getKey (void);

#endif /* OUTPUT_H_ */
