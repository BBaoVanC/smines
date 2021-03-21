CFLAGS = -Wall -std=c99
LDFLAGS = -lncurses

OBJECTS=smines.o minefield.o window.o draw.o helper.o help.o

all: smines

run: clean smines
	./smines

debug: clean smines.out

dbgrun: debug
	./smines.out

valgrind: clean smines.out
	valgrind --leak-check=full --log-file=vgdump ./smines.out


smines: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o smines

smines.out:
	$(CC) $(CFLAGS) -g -Og $(LDFLAGS) $(SOURCES) -o smines.out

$(OBJECTS): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o
	rm -f smines smines.out

.PHONY: all run debug dbgrun valgrind clean
