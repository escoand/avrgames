#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <pthread.h>

#include "input.h"
#include "device.h"


uint32_t        keyMapping[] = { DEVICE_KEY_MAPPING };
uint32_t        lastButton = BUTTON_NONE;


void
readEvents(char *name)
{
    FILE           *fp;
    struct input_event ev;

    fp = fopen(name, "r");
    if (fp == NULL)
	return;

#ifdef DEBUG
    printf("read input events from %s\n", name);
#endif

    while (1) {
	if (fread(&ev, sizeof(ev), 1, fp) == 0)
	    break;
	if (ev.type != EV_KEY)
	    continue;

	for (uint8_t i = 0; i < sizeof(keyMapping) / sizeof(uint32_t);
	     i += 2) {
	    if (keyMapping[i] == ev.code) {
		lastButton = keyMapping[++i];
#ifdef DEBUG
		printf("input event code=0x%02X value=0x%0X is 0x%0X\n",
		       ev.code, ev.value, lastButton);
#endif
		break;
	    }
	}
#ifdef DEBUG
	printf("input event code=0x%02X value=0x%0X\n", ev.code, ev.value);
#endif
    }
    fclose(fp);
}

void
               *
initInput_(void *arg)
{
#ifdef DEVICE_FILE
    while (1) {
	readEvents(DEVICE_FILE);
	sleep(DEVICE_READ_DELAY);
    }
#else
    DIR            *dp;
    struct dirent  *ep;
    uint8_t         name[50];

    dp = opendir(DEVICE_DIR);
    if (dp == NULL) {
	perror("Couldn't open the directory");
	return;
    }

    while (1) {
	while (ep = readdir(dp)) {
	    if (strncmp(ep->d_name, "event,", 5) == 0) {
		sprintf(name, "%s/%s\0", DEVICE_DIR, ep->d_name);
		readEvents(name);
	    }
	}
	rewinddir(dp);
	sleep(DEVICE_READ_DELAY);
    }
    closedir(dp);
#endif
}

void
initInput(void)
{
    pthread_t       pth;

    pthread_create(&pth, NULL, initInput_, NULL);
}

uint32_t
getInput(void)
{
    uint32_t        button = lastButton;

    lastButton = BUTTON_NONE;

    return button;
}
