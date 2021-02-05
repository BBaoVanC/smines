/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include "minefield.h"
#include "logger.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


Minefield *minefield;

int main() {
    srand((unsigned) time(NULL));
    FILE *logfile = fopen("full.log", "w+");

    Minefield *minefield = malloc(sizeof(Minefield));
    logger(logfile, "debug", "Setting up the minefield\n");
    init_minefield(minefield, 10, 20);
    minefield->mines = 5;

    int i, x, y;
    Tile *t;
    for (i = 0; i < minefield->mines;) {
        x = (rand() % (minefield->cols - 1 + 1));
        y = (rand() % (minefield->rows - 1 + 1));
        t = get_tile_ptr(minefield, x, y);
        if (!t->mine) {
            t->mine = true;
            t->visible = true; /* TODO remove this */
            i++;
        }
    }

    for (y = 0; y < minefield->rows; y++) {
        for (x = 0; x < minefield->cols; x++) {
            t = get_tile_ptr(minefield, x, y);
            if (!t->mine) {
                t->surrounding = getsurround(minefield, x, y);
                t->visible = true; /* TODO remove this */
            }
        }
    }


    printf("Initializing screen\n");
    initscr(); /* start ncurses */
    printf("Enabling extra keys with keypad()\n");
    keypad(stdscr, TRUE); /* more keys */
    printf("Hiding key output\n");
    noecho(); /* hide keys when pressed */

    printf("Setting up color\n");
    start_color(); /* start color */
    printf("Initializing color pairs\n");

    /* 10 is the 0 tile (0 can't be used for the pair number) */
    /* 9 is a mine */
    init_pair(10, COLOR_WHITE, COLOR_BLACK);
    init_pair(9, COLOR_BLACK, COLOR_RED);

    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_GREEN);
    init_pair(3, COLOR_WHITE, COLOR_RED);
    init_pair(4, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(5, COLOR_WHITE, COLOR_YELLOW);
    init_pair(6, COLOR_WHITE, COLOR_CYAN);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
    init_pair(8, COLOR_BLACK, COLOR_WHITE);

    /* 11 is a hidden tile */
    /* 12 is a flag */
    init_pair(11, COLOR_WHITE, COLOR_BLACK);
    init_pair(12, COLOR_WHITE, COLOR_YELLOW);

    /* 100 is for errors */
    init_pair(100, COLOR_WHITE, COLOR_RED);
            

    print_minefield(minefield);
    refresh();

    bool keeprunning = true;
    int ch;
    while (keeprunning) {
        ch = getch();
        switch (ch) {
            case 'q':
                keeprunning = false;
                break;
        }

        clear();
        print_minefield(minefield);
        refresh();
    }

    endwin();
}
