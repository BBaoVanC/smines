CFLAGS = -Wall -std=c99
LDFLAGS = -lncurses

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	CFLAGS += -D_POSIX_C_SOURCE
endif

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

smines.out:
	$(CC) $(CFLAGS) -g -Og $(LDFLAGS) $(SOURCES) -o smines.out

$(OBJECTS):
	$(CC) $(CFLAGS) -c $(SOURCES)

clean:
	rm -f *.o
	rm -f smines smines.out

.PHONY: all run debug dbgrun clean
