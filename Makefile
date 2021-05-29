CFLAGS += -Wall -std=c99
LDFLAGS += -lncurses

SOURCES = smines.c minefield.c window.c draw.c helper.c help.c undo.c
OBJECTS = $(SOURCES:.c=.o)

PREFIX ?= /usr

all: smines

install: smines
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m 0755 smines $(DESTDIR)$(PREFIX)/bin/smines

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/smines

config.h: config.def.h
	cp config.def.h config.h

.c.o:
	$(CC) $(CFLAGS) -c $<

draw.o: colornames.h types.h
helper.o: global.h
minefield.o: colornames.h types.h config.h
smines.o: morecolor.h colornames.h types.h global.h
undo.o: global.h

$(OBJECTS): config.h

smines: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(LDFLAGS)

run: smines
	./smines

debug: CFLAGS += -g -Og
debug: clean smines

dbgrun: debug
	./smines

valgrind: debug
	valgrind $(VGFLAGS) --leak-check=full --log-file=vgdump ./smines

clean:
	rm -f *.o
	rm -f smines

.PHONY: all run debug dbgrun valgrind clean
