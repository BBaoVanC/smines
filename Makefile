all: smines
dev: clean smines

smines:
	gcc -o smines smines.c -lncurses

clean:
	rm -f smines

.PHONY: all clean
