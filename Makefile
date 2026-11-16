CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -O2

.PHONY: all clean
all: banker

banker: main.c arguments.c customers.c banker.h arguments.h customers.h
	$(CC) $(CFLAGS) main.c arguments.c customers.c -o banker

clean:
	rm -f banker
