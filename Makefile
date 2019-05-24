TARGET   = avrgames
SRC      = main.c \
           input/device.c \
           input/gpio.c \
           input/input.c \
           input/mqtt.c \
           input/terminal.c \
           output/gpio.c \
           games/loading.c \
           games/menu.c \
           games/clock.c \
           games/tetris.c \
           games/fire.c \
           log/src/log.c
OBJ      = $(SRC:.c=.o)
CFLAGS   = -Wall -ggdb -O2 \
           -Ilog/src -DLOG_USE_COLOR -g \
           -Imosquitto/lib \
           -Irpi_ws281x
LDFLAGS  = -Lmosquitto/lib -lmosquitto -pthread \
           -Lrpi_ws281x -lws2811

# terminal
TERMSRC := $(filter-out $(wildcard output/*.c),$(SRC)) \
           output/terminal.c
TERMOBJ  = $(TERMSRC:.c=.o)
TERMLDF  = -lmosquitto

# clear
CLRSRC   = clear.c \
           output/gpio.c \
           log/src/log.c
CLROBJ   = $(CLRSRC:.c=.o)

CC       = gcc
RM       = rm -f
INSTALL  = install -o root

.PHONY: indent libs install uninstall clean clean-libs

# default target
all: $(TARGET) clear

# source
%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<

# targets
$(TARGET): libs $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

terminal: $(TERMOBJ)
	$(CC) -o $(TARGET) $^ $(TERMLDF)

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
	make -C mosquitto/lib libmosquitto.a WITH_TLS=no WITH_TLS_PSK=no
	scons -C rpi_ws281x

# clean
clean:
	$(RM) *.[ch]~ */*.[ch]~ *.o */*.o $(TARGET) clear

clean-libs:
	make -C mosquitto/lib clean
	scons -C rpi_ws281x --clean

# helper
indent:
	indent -orig *.c input/*.c input/*.h output/*.c output/*.h games/*.c games/*.h
