TARGET  = ledmatrix
SRC     = main.c \
          input/terminal.c \
          output/gpio.c \
          games/clock.c \
          games/tetris.c
OBJ     = $(SRC:.c=.o)
CFLAGS  = -Irpi_ws281x -DDEBUG
LDFLAGS = -Lrpi_ws281x -lws2811

CC = gcc
RM = rm -f

# default target
all: libs $(TARGET) clear

# source
%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<

# targets
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clear: clear.o output/gpio.o
	$(CC) -o $@ $^ $(LDFLAGS)

# libs
.PHONY: libs
libs:
	scons -C rpi_ws281x

# clean
.PHONY: clean
clean:
	$(RM) *.o */*.o $(TARGET) clear

.PHONY: clean-libs
clean-libs:
	scons -C rpi_ws281x --clean

# helper
.PHONY: indent
indent:
	indent -orig *.c input/*.c input/*.h output/*.c output/*.h games/*.c games/*.h

