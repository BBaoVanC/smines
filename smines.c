/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include "minefield.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main() {
    srand((unsigned) time(NULL)); /* create seed */
    FILE *logfile = fopen("full.log", "w+");

    fprintf(logfile, "debug: Setting up the minefield");
    Minefield *minefield = init_minefield(MROWS, MCOLS, MINES);

    int i, x, y;
    Tile *t = NULL;
    for (i = 0; i < minefield->mines;) {
        fprintf(logfile, "debug: for loop");
        y = (rand() % (minefield->rows - 1 + 1)); /* generate random y */
        x = (rand() % (minefield->cols - 1 + 1)); /* generate random x */
        t = &minefield->tiles[y][x];
        if (!t->mine) {
            t->mine = true;
            t->visible = true; /* TODO remove this */
            i++;
        }
    }

#if 1 /* TODO remove this */
    for (y = 0; y < minefield->rows; y++) {
        for (x = 0; x < minefield->cols; x++) {
            minefield->tiles[y][x].visible = true;
        }
    }
#endif

    generate_surrounding(minefield); /* set the surrounding value in each tile */


    fprintf(logfile, "debug: Initializing screen\n");
    initscr(); /* start ncurses */
    fprintf(logfile, "debug: Enabling extra keys with keypad()\n");
    keypad(stdscr, TRUE); /* more keys */
    fprintf(logfile, "debug: Hiding key output\n");
    noecho(); /* hide keys when pressed */

    fprintf(logfile, "debug: Setting up color\n");
    start_color(); /* start color */
    fprintf(logfile, "debug: Initializing color pairs\n");

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
    init_pair(11, COLOR_BLACK, COLOR_WHITE);
    init_pair(12, COLOR_WHITE, COLOR_YELLOW);

    /* 100 is for errors */
    init_pair(100, COLOR_WHITE, COLOR_RED);


    print_minefield(minefield);
    refresh();

    bool keeprunning = true;
    int ch;
    while (keeprunning) {
        ch = getch(); /* wait for a character press */
        switch (ch) {
            case 'q': /* quit */
                keeprunning = false;
                break;
        }

        clear();
        print_minefield(minefield);
        refresh();
    }

    endwin();
    free(minefield);
}
