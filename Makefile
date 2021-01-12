all: smines

smines:
	gcc -o smines smines.c -lncurses

clean:
	rm -f smines

.PHONY: all clean
