TARGET    = avrgames
SRC       = main.c \
            input/device.c \
            input/gpio.c \
            input/input.c \
            input/mqtt.c \
            input/terminal.c \
            games/loading.c \
            games/menu.c \
            games/clock.c \
            games/tetris.c \
            games/fire.c \
            log/src/log.c
OBJ       = $(SRC:.c=.o)
CFLAGS    = -Wall -ggdb -O2 -DBUILDING_DLL \
            -Ilog/src -DLOG_USE_COLOR -g \
            -Imosquitto/lib \
            -Irpi_ws281x
LDFLAGS   = -Lmosquitto/lib -lmosquitto -pthread \
            -Lrpi_ws281x -lws2811

# target specific
DEFLOBJ   = output/terminal.o
GPIOOBJ   = output/gpio.o
CLROBJ    = clear.o \
            output/gpio.o \
            log/src/log.o

RM        = rm -f
INSTALL   = install -o root

# override variables
CC       := $(CROSS_PREFIX)$(CC)$(CROSS_SUFFIX)
AR       := $(CROSS_PREFIX)$(AR)$(CROSS_SUFFIX)

.PHONY:  indent libs install uninstall clean clean-libs

# default target
all: $(TARGET) gpio clear

# source
%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<

# targets
$(TARGET): libs $(OBJ) $(DEFLOBJ)
	$(CC) -o $@ $(OBJ) $(DEFLOBJ) $(LDFLAGS)

gpio: libs $(OBJ) $(GPIOOBJ)
	$(CC) -o $@ $(OBJ) $(GPIOOBJ) $(LDFLAGS)

clear: libs $(CLROBJ)
	$(CC) -o $@ $(CLROBJ) $(LDFLAGS)

# install
install: $(TARGET)
	$(INSTALL) $< /usr/local/bin/
	$(INSTALL) etc/$(TARGET).service /etc/systemd/system/
	systemctl enable $(TARGET)

# uninstall
uninstall:
	systemctl disable $(TARGET)
	$(RM) /usr/local/bin/$(TARGET) /etc/systemd/system/$(TARGET).service

# libs
libs:
	make -C mosquitto/lib libmosquitto.a WITH_TLS=no WITH_TLS_PSK=no WITH_THREADING=no CC=$(CC) AR=$(AR)
	scons -C rpi_ws281x TOOLCHAIN=$(CROSS_PREFIX:-=) CC=$(CC) AR=$(AR)

# clean
clean:
	$(RM) *.[ch]~ */*.[ch]~ *.o */*.o $(TARGET) gpio clear

clean-libs:
	make -C mosquitto/lib clean
	scons -C rpi_ws281x --clean

# helper
indent:
	indent -orig *.c input/*.c input/*.h output/*.c output/*.h games/*.c games/*.h
