TARGET   = ledmatrix
SRC      = main.c \
           input/device.c \
           output/gpio.c \
           games/loading.c \
           games/menu.c \
           games/clock.c \
           games/tetris.c \
           games/fire.c \
           log/src/log.c
OBJ      = $(SRC:.c=.o)
CFLAGS   = -Irpi_ws281x -Ilog/src -DLOG_USE_COLOR -g
LDFLAGS  = -Lrpi_ws281x -lws2811

# terminal
TERMSRC := $(filter-out $(wildcard */gpio.c */device.c),$(SRC)) \
           input/terminal.c \
           output/terminal.c
TERMOBJ  = $(TERMSRC:.c=.o)
TERMLDF  =

# clear
CLRSRC   = clear.c \
	   output/gpio.c \
	   log/src/log.c
CLROBJ   = $(CLRSRC:.c=.o)

CC       = gcc
RM       = rm -f
INSTALL  = install -o root

# default target
all: $(TARGET) clear

# source
%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<

# targets
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

terminal: $(TERMOBJ)
	$(CC) -o $(TARGET) $^ $(TERMLDF)

clear: $(CLROBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# install
.PHONY: install
install: $(TARGET)
	$(INSTALL) $< /usr/local/bin/
	$(INSTALL) etc/$(TARGET).service /etc/systemd/system/
	systemctl enable $(TARGET)

# uninstall
.PHONY: uninstall
uninstall:
	systemctl disable $(TARGET)
	$(RM) /usr/local/bin/$(TARGET) /etc/systemd/system/$(TARGET).service

# libs
.PHONY: libs
libs:
	scons -C rpi_ws281x

# clean
.PHONY: clean
clean:
	$(RM) *.[ch]~ */*.[ch]~ *.o */*.o $(TARGET) clear

.PHONY: clean-libs
clean-libs:
	scons -C rpi_ws281x --clean

# helper
.PHONY: indent
indent:
	indent -orig *.c input/*.c input/*.h output/*.c output/*.h games/*.c games/*.h

