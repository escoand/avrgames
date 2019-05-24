#ifndef INPUT_H_
#define INPUT_H_

#include <stddef.h>
#include <stdint.h>

enum inputButton {
    BUTTON_NONE = 0,
    BUTTON_LEFT = 1,
    BUTTON_RIGHT = 2,
    BUTTON_UP = 4,
    BUTTON_DOWN = 8,
    BUTTON_MENU = 16
};

struct inputModules {
    size_t length;
    void (*init[8]) (void);
    uint32_t (*read[8]) (void);
};

void initInputs(void) ;

uint32_t getInputs(void);

#endif				/* INPUT_H_ */
