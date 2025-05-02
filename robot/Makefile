CC = gcc
CFLAGS = -Wall -Wextra -I.
LDFLAGS = 

SRCS = main.c
OBJS = $(addprefix Compiledir/,$(SRCS:.c=.o))
TARGET = Compiledir/robot

.PHONY: all clean

all: Compiledir $(TARGET)

Compiledir:
	mkdir -p Compiledir

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

Compiledir/%.o: %.c | Compiledir
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf Compiledir 