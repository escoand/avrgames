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
CFLAGS    = -Wall -ggdb -O2 \
            -Ilog/src -DLOG_USE_COLOR -g \
            -Imosquitto/lib \
            -Irpi_ws281x
LDFLAGS   = -Lmosquitto/lib -lmosquitto -pthread \
            -Lrpi_ws281x -lws2811

# target specific
DEFLOBJ   = output/terminal.o
GPIOOBJ   = output/gpio.o
CLROBJ    = clear.0 \
            output/gpio.0 \
            log/src/log.0
WS281xOBJ = rpi_ws281x/ws2811.o \
            rpi_ws281x/pwm.o \
            rpi_ws281x/pcm.o \
            rpi_ws281x/dma.o \
            rpi_ws281x/rpihw.o

RM        = rm -f
INSTALL   = install -o root

.PHONY:  indent libs install uninstall clean clean-libs
.IGNORE: gpio ws281x

# default target
all: $(TARGET) gpio clear

# source
%.o : %.c
	${CROSS_COMPILE}$(CC) $(CFLAGS) -o $@ -c $<

# targets
$(TARGET): libs $(OBJ) $(DEFLOBJ)
	${CROSS_COMPILE}$(CC) -o $@ $(OBJ) $(DEFLOBJ) $(LDFLAGS)

gpio: libs $(OBJ) $(GPIOOBJ)
	${CROSS_COMPILE}$(CC) -o $@ $(OBJ) $(GPIOOBJ) $(LDFLAGS)

clear: libs $(CLROBJ)
	${CROSS_COMPILE}$(CC) -o $@ $(CLROBJ) $(LDFLAGS)

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
libs: ws281x
	make -C mosquitto/lib libmosquitto.a WITH_TLS=no WITH_TLS_PSK=no
ws281x: $(WS281xOBJ)
	${CROSS_COMPILE}$(CC) -o $@ $^ $(LDFLAGS)

# clean
clean:
	$(RM) *.[ch]~ */*.[ch]~ *.o */*.o $(TARGET) gpio clear

clean-libs:
	make -C mosquitto/lib clean
	scons -C rpi_ws281x --clean

# helper
indent:
	indent -orig *.c input/*.c input/*.h output/*.c output/*.h games/*.c games/*.h
