#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <stdint.h>

#define BOARD_WIDTH          10
#define BOARD_HEIGHT         18


#define OUTPUT_COLORS_COUNT  7

#define BOARD_COLOR_NONE     0
#define BOARD_COLOR_WHITE    1
#define BOARD_COLOR_RED      2
#define BOARD_COLOR_GREEN    3
#define BOARD_COLOR_BLUE     4
#define BOARD_COLOR_YELLOW   5
#define BOARD_COLOR_PINK     6
#define BOARD_COLOR_CYAN     7


typedef uint8_t board_matrix[BOARD_HEIGHT][BOARD_WIDTH];

void            initOutput(void);
void            setOutput(board_matrix * board);
void            clearOutput(void);

#endif				/* OUTPUT_H_ */
