#ifndef INPUT_DEVICE_H_
#define INPUT_DEVICE_H_

#include <linux/input.h>

#define DEVICE_DIR          "/dev/input"
#define DEVICE_FILE         "/dev/input/event0"
#define DEVICE_READ_DELAY   5

#define DEVICE_KEY_MAPPING  KEY_LEFT,  BUTTON_LEFT, \
                            KEY_RIGHT, BUTTON_RIGHT, \
                            KEY_UP,    BUTTON_UP, \
                            KEY_DOWN,  BUTTON_DOWN, \
                            KEY_A,     BUTTON_LEFT, \
                            KEY_D,     BUTTON_RIGHT, \
                            KEY_W,     BUTTON_UP, \
                            KEY_S,     BUTTON_DOWN

#endif				/* INPUT_DEVICE_H_ */
