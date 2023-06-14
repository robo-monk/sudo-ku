CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS =

SOURCES = bitboard.c main.c sudoku.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = program

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
