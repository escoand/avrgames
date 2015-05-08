#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <inttypes.h>

#define PORT_STRIPE          B
#define PIN_STRIPE           2

#define BOARD_WIDTH          10
#define BOARD_HEIGHT         18
#define BOARD_STRIPE_MODE    3	// 0=top-bottom 1=bottom-top 2=chain-from-top 3=chain-from-bottom

#define OUTPUT_COLORS_COUNT  6
#define OUTPUT_BLOCK_SIZE    (2*BOARD_HEIGHT)


#define CONCAT(a, b)         a ## b
#define CONCAT_EXP(a, b)     CONCAT(a, b)


#define DDRREG_STRIPE        CONCAT_EXP(DDR,PORT_STRIPE)
#define PORTREG_STRIPE       CONCAT_EXP(PORT,PORT_STRIPE)


typedef uint8_t board_matrix[BOARD_HEIGHT][BOARD_WIDTH];

void initOutput (void);
void output (board_matrix * board);
uint8_t getKey (void);

#endif /* OUTPUT_H_ */
