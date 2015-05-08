#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <inttypes.h>

#define BOARD_WIDTH  10
#define BOARD_HEIGHT 18
#define BOARD_STRIPE_MODE 3	// 0=top-bottom 1=bottom-top 2=chain-from-top 3=chain-from-bottom
#define OUTPUT_COLORS_COUNT 6

void initOutput (void);
void output (uint8_t board[BOARD_HEIGHT][BOARD_WIDTH]);
uint8_t getKey (void);

#endif /* OUTPUT_H_ */
