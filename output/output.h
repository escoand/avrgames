#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <stdint.h>

#define BOARD_WIDTH          10
#define BOARD_HEIGHT         18

#define OUTPUT_COLORS_COUNT  6

typedef uint8_t board_matrix[BOARD_HEIGHT][BOARD_WIDTH];

void            initOutput(void);
void            setOutput(board_matrix * board);
void            clearOutput(void);

#endif				/* OUTPUT_H_ */
