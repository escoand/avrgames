#include <stdint.h>

#include "input.h"
#include "gpio.h"

void
initInput(void)
{
}

uint32_t
getInput(void)
{
    uint32_t        button = BUTTON_NONE;

#ifdef __AVR__
    if (bit_is_clear(PINREG_LEFT, PINBIT_LEFT))
	button = button | BUTTON_LEFT;
    else if (bit_is_clear(PINREG_RIGHT, PINBIT_RIGHT))
	button = button | BUTTON_RIGHT;
    else if (bit_is_clear(PINREG_UP, PINBIT_UP))
	button = button | BUTTON_UP;
    else if (bit_is_clear(PINREG_DOWN, PINBIT_DOWN))
	button = button | BUTTON_DOWN;
#else
#endif

    return button;
}
