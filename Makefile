CFLAGS = -Wall -g
LDFLAGS = -lncurses

all: clean smines

run: clean smines
	./smines

smines:
	$(CC) $(CFLAGS) $(LDFLAGS) smines.c minefield.c -o smines

clean:
	rm -f smines

.PHONY: all run clean
