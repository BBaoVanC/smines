CFLAGS = -Wall -g -std=c99
LDFLAGS = -lncurses

SOURCES=smines.c minefield.c
OBJECTS=smines.o minefield.o

all: smines

run: clean smines
	./smines

smines: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) -o smines

$(OBJECTS):
	$(CC) $(CFLAGS) -c $(SOURCES)

clean:
	rm -f *.o
	rm -f smines

.PHONY: all run clean
