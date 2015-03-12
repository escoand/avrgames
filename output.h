#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <inttypes.h>

#define BOARD_WIDTH  10
#define BOARD_HEIGHT 18
#define OUTPUT_COLORS_COUNT 6

void initOutput ();
void output (uint8_t board[BOARD_WIDTH][BOARD_HEIGHT]);
uint8_t getKey ();

#endif /* OUTPUT_H_ */
