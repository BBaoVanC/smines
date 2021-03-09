CFLAGS = -Wall -std=c99
LDFLAGS = -lncurses

SOURCES=smines.c minefield.c
OBJECTS=smines.o minefield.o

all: smines

run: clean smines
	./smines

debug: clean smines.out

dbgrun: debug
	./smines.out


smines: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o smines

smines.out: $(OBJECTS)
	$(CC) $(CFLAGS) -g -Og $(LDFLAGS) $(OBJECTS) -o smines.out

$(OBJECTS):
	$(CC) $(CFLAGS) -c $(SOURCES)

clean:
	rm -f *.o
	rm -f smines smines.out

.PHONY: all run debug dbgrun clean
