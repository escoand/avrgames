#ifndef INPUT_H_
#define INPUT_H_

#include <stdint.h>

enum inputButton {
    BUTTON_NONE = 0,
    BUTTON_LEFT = 1,
    BUTTON_RIGHT = 2,
    BUTTON_UP = 4,
    BUTTON_DOWN = 8,
    BUTTON_MENU = 16
};

void            initInput(void);
uint32_t        getInput(void);

#endif				/* INPUT_H_ */
