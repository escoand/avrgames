TARGET         = tetris
SOURCES        = main.c output.c tetris.c
OBJECTS        =$(SOURCES:.c=.o)

CC             = gcc
CFLAGS         = -Os -Wall
LDFLAGS        =

F_CPU          = 16000000
DEVICE         = atmega8

AVRLIB         = light_ws2812
AVRLIBDIR      = light_ws2812/light_ws2812_AVR

debug: CCFLAGS+= -g -D_DEBUG -D_DEBUG_
debug: all

avr: CC        = avr-gcc
avr: CFLAGS    = -g2 -mmcu=$(DEVICE) -DF_CPU=$(F_CPU) -I$(AVRLIBDIR) -I$(AVRLIBDIR)/Light_WS2812
avr: CFLAGS   += -Os -ffunction-sections -fdata-sections -fpack-struct -fno-move-loop-invariants -fno-tree-scev-cprop -fno-inline-small-functions
avr: CFLAGS   += -Wall -Wno-pointer-to-int-cast
avr: LDFLAGS   = -Wl,--relax,--section-start=.text=0,-Map=main.map*/

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	
avr: $(OBJECTS)
	$(MAKE) -C $(AVRLIBDIR) $(AVRLIB)
	$(CC) $(CFLAGS) -o $(TARGET).o $(TARGET).c
	avr-size $(TARGET).o
	avr-objcopy -j .text -j .data -O ihex $(TARGET).o $(TARGET).hex
	avr-objdump -d -S $(TARGET).o >$(TARGET).lss

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY:	clean

clean:
	$(RM) *.o $(TARGET)
	$(MAKE) -C $(AVRLIBDIR) clean
