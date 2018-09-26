TARGET  = game
SRC     = main.c \
          input/terminal.c \
          output/gpio.c \
          games/tetris.c \
          games/qlock.c
OBJ     = $(SRC:.c=.o)
CFLAGS  = 
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
clean: clean-libs
	$(REMOVE) *.o */*.o $(TARGET)

.PHONY: clean-libs
clean-libs:
	scons -C rpi_ws281x --clean
