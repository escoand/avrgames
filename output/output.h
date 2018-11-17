#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <stdint.h>

#define BOARD_WIDTH          10
#define BOARD_HEIGHT         18

enum BOARD_PALETTE {
    BOARD_PALETTE_DEFAULT, BOARD_PALETTE_RAINBOW, BOARD_PALETTE_FIRE
};

typedef uint8_t board_matrix[BOARD_HEIGHT][BOARD_WIDTH];

void            initOutput(void);
void            setOutput(board_matrix *);
void            setOutputUsePalette(board_matrix *, enum BOARD_PALETTE);
void            clearOutput(void);

#endif				/* OUTPUT_H_ */
