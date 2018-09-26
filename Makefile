TARGET  = game
SRC     = main.c \
          input/terminal.c \
          output/gpio.c \
          games/tetris.c \
          games/clock.c
OBJ     = $(SRC:.c=.o)
CFLAGS  = -DDEBUG
LDFLAGS = -Lrpi_ws281x -lws2811

CC = gcc
REMOVE = rm -f

all: libs $(TARGET)

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: libs
libs:
	scons -C rpi_ws281x

.PHONY: clean
clean:
	$(REMOVE) *.o */*.o $(TARGET)

.PHONY: clean-libs
clean-libs:
	scons -C rpi_ws281x --clean

.PHONY: indent
indent:
	indent -orig *.c input/*.c input/*.h output/*.c output/*.h games/*.c games/*.h

