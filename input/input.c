#include "device.h"
#include "gpio.h"
#include "input.h"
#include "mqtt.h"
#include "terminal.h"

struct inputModules inputs;

void
initInputs(void) {
    inputs.length = 0;
    inputs.init[inputs.length] = device_init;
    inputs.read[inputs.length++] = device_getkey;
    inputs.init[inputs.length] = gpio_init;
    inputs.read[inputs.length++] = device_getkey;
    inputs.init[inputs.length] = mqtt_init;
    inputs.read[inputs.length++] = mqtt_loop;
    inputs.init[inputs.length] = terminal_init;
    inputs.read[inputs.length++] = terminal_getkey;
}

uint32_t
getInputs(void) {
    uint32_t button = BUTTON_NONE;

    for(uint8_t i = 0; i < inputs.length; i++)
	button |= (*inputs.read[i]) ();

    return button;
}
