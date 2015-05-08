#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <inttypes.h>

#define PORT_LEDS    PORTB
#define PIN_LEDS     1

#define BOARD_WIDTH  10
#define BOARD_HEIGHT 18
#define BOARD_STRIPE_MODE 3	// 0=top-bottom 1=bottom-top 2=chain-from-top 3=chain-from-bottom
#define OUTPUT_COLORS_COUNT 6

typedef uint8_t board_matrix[BOARD_HEIGHT][BOARD_WIDTH];

void initOutput (void);
void output (board_matrix *board);
uint8_t getKey (void);

#endif /* OUTPUT_H_ */
