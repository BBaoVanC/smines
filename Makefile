all: clean smines

smines:
	gcc -o smines smines.c -lncurses minefield.c colors.c

clean:
	rm -f smines

.PHONY: all clean
