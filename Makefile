CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -O2

.PHONY: all clean
all: banker

banker: main.c
	$(CC) $(CFLAGS) main.c -o banker

clean:
	rm -f banker
