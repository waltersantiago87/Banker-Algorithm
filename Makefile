CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -O2

.PHONY: all clean
all: banker

banker: main.c arguments.c customers.c matrices.c safety.c request.c banker.h arguments.h customers.h matrices.h safety.h request.h
	$(CC) $(CFLAGS) main.c arguments.c customers.c matrices.c safety.c request.c -o banker

clean:
	rm -f banker
