#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <inttypes.h>

#define BOARD_WIDTH  4
#define BOARD_HEIGHT 18

void output (struct cRGB board[BOARD_HEIGHT][BOARD_WIDTH]);
uint8_t getKey (void);

#endif /* OUTPUT_H_ */
