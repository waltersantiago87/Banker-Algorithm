CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -O2

.PHONY: all clean
all: banker

banker: main.c arguments.c customers.c matrices.c safety.c request.c release.c state.c parser.c commands.c output.c banker.h arguments.h customers.h matrices.h safety.h request.h release.h state.h parser.h commands.h output.h
	$(CC) $(CFLAGS) main.c arguments.c customers.c matrices.c safety.c request.c release.c state.c parser.c commands.c output.c -o banker

clean:
	rm -f banker
