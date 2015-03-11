#ifndef TETRIS_H_
#define TETRIS_H_

#include <inttypes.h>
#include "output.h"

uint8_t nextStep (uint8_t board[BOARD_WIDTH][BOARD_HEIGHT], uint8_t key);

#endif /* TETRIS_H_ */
