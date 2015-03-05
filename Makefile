PROGNAME  = tetris

F_CPU     = 16000000
DEVICE    = atmega8

AVRLIB    = light_ws2812
AVRLIBDIR = light_ws2812/light_ws2812_AVR

CC        = avr-gcc
CFLAGS    = -g2 -mmcu=$(DEVICE) -DF_CPU=$(F_CPU) -I$(AVRLIBDIR) -I$(AVRLIBDIR)/Light_WS2812
CFLAGS   += -Os -ffunction-sections -fdata-sections -fpack-struct -fno-move-loop-invariants -fno-tree-scev-cprop -fno-inline-small-functions
CFLAGS   += -Wall -Wno-pointer-to-int-cast
LDFLAGS  += -Wl,--relax,--section-start=.text=0,-Map=main.map

all:	avr

avr:
	$(MAKE) -C $(AVRLIBDIR) $(AVRLIB)
	$(CC) $(CFLAGS) -o $(PROGNAME).o $(PROGNAME).c
	avr-size $(PROGNAME).o
	avr-objcopy -j .text -j .data -O ihex $(PROGNAME).o $(PROGNAME).hex
	avr-objdump -d -S $(PROGNAME).o >$(PROGNAME).lss

.PHONY:	clean

clean:
	$(MAKE) -C $(AVRLIBDIR) clean
