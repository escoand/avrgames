#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#include "input.h"
#include "device.h"
#include "../log.h"


uint32_t        keyMapping[] = { DEVICE_KEY_MAPPING };


void
initInput(void)
{
}

uint32_t
getInput(void)
{
    static int      fd = 0;
    uint8_t         max = sizeof(keyMapping) / sizeof(uint32_t);
    uint32_t        button = BUTTON_NONE;
    struct input_event ev;

    if (fd == 0) {
#ifdef DEVICE_FILE
	fd = open(DEVICE_FILE, O_RDONLY | O_NONBLOCK);
	LOGI("open input device file %s", DEVICE_FILE);
#else
	DIR            *dp;
	struct dirent  *ep;
	uint8_t         name[50];

	dp = opendir(DEVICE_DIR);
	if (dp == NULL) {
	    LOGW("Couldn't open input device directory");
	    return button;
	}

	while (ep = readdir(dp)) {
	    if (strncmp(ep->d_name, "event,", 5) == 0) {
		sprintf(name, "%s/%s\0", DEVICE_DIR, ep->d_name);
		fd = open(name, O_RDONLY | O_NONBLOCK);
		LOGI("open input device file %s", name);
	    }
	}
	closedir(dp);
#endif
    }

    if (fd == 0)
	return button;

    while (read(fd, &ev, sizeof(ev)) > 0) {
	if (ev.type != EV_KEY)
	    continue;

	for (uint8_t i = 0; i < max; i += 2) {
	    if (keyMapping[i] == ev.code) {
		button = button | keyMapping[++i];
		LOGD("input event code=0x%02X value=0x%0X is 0x%0X",
		     ev.code, ev.value, keyMapping[i]);
		break;
	    }
	}
	LOGD("input event code=0x%02X value=0x%0X", ev.code, ev.value);
    }

    if (errno != EAGAIN) {
	close(fd);
	fd = 0;
	LOGI("close input device file");
    }

    return button;
}
