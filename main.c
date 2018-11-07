#include <stdint.h>
#include <unistd.h>

#ifdef __AVR__
#include <avr/io.h>
#endif

#include "input/input.h"
#include "output/output.h"
#include "games/loading.h"
#include "games/menu.h"
#include "games/clock.h"

int
main(void)
{
    uint32_t        button;

#ifdef CLKPR
    CLKPR = _BV(CLKPCE);
    CLKPR = 0;
#endif

    initInput();
    initOutput();

    // init device
    while (clock_main() != CLOCK_RETURN_SUCCESS) {
	loading_main();
    }

    // default sceen
    while (1) {
	button = getInput();

	if (button & BUTTON_MENU) {
	    menu_main();
	} else {
	    clock_main();
	    sleep(1);
	}
    }

    clearOutput();
}
