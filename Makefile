CFLAGS = -Wall -g -std=c99
LDFLAGS = -lncurses

SOURCES=smines.c minefield.c
OBJECTS=smines.o minefield.o

all: smines

run: clean smines
	./smines

debug: clean smines.out


smines: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o smines

smines.out: $(OBJECTS)
	$(CC) $(CFLAGS) -Og $(LDFLAGS) $(OBJECTS) -o smines.out

$(OBJECTS):
	$(CC) $(CFLAGS) -c $(SOURCES)

clean:
	rm -f *.o
	rm -f smines smines.out

.PHONY: all run debug clean
