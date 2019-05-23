#ifndef INPUT_H_
#define INPUT_H_

#include <stdint.h>

#define addInput(a,b) \
        inputs.init[inputs.length] = a; \
        inputs.read[inputs.length] = b; \
        inputs.length++;

enum inputButton {
    BUTTON_NONE = 0,
    BUTTON_LEFT = 1,
    BUTTON_RIGHT = 2,
    BUTTON_UP = 4,
    BUTTON_DOWN = 8,
    BUTTON_MENU = 16
};

struct {
    size_t length;
    void (*init[8]) (void);
    uint32_t (*read[8]) (void);
} inputs;
inputs.length = 0;

#endif				/* INPUT_H_ */
