/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include "minefield.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void death(Minefield *minefield) {
    reveal_mines(minefield);
}

int main() {
    srand((unsigned) time(NULL)); /* create seed */

    /* ncurses setup */
    initscr(); /* start ncurses */
    keypad(stdscr, TRUE); /* more keys */
    noecho(); /* hide keys when pressed */

    start_color(); /* start color */

    init_pair(10, COLOR_WHITE, COLOR_BLACK);    /* 0 */
    init_pair(9, COLOR_BLACK, COLOR_RED);       /* mine */

    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_GREEN);
    init_pair(3, COLOR_WHITE, COLOR_RED);
    init_pair(4, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(5, COLOR_WHITE, COLOR_YELLOW);
    init_pair(6, COLOR_WHITE, COLOR_CYAN);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
    init_pair(8, COLOR_BLACK, COLOR_WHITE);

    init_pair(11, COLOR_BLACK, COLOR_BLACK);    /* hidden tile */
    init_pair(12, COLOR_BLACK, COLOR_YELLOW);   /* flag */
    init_pair(13, COLOR_BLACK, COLOR_WHITE);    /* cursor */

    /* 100 is for errors */
    init_pair(100, COLOR_WHITE, COLOR_RED);

    /* done with ncurses setup */


    Minefield *minefield = init_minefield(MROWS, MCOLS, MINES);

    int i, x, y;
    Tile *t = NULL;
    for (i = 0; i < minefield->mines;) {
        y = (rand() % (minefield->rows - 1 + 1)); /* generate random y */
        x = (rand() % (minefield->cols - 1 + 1)); /* generate random x */
        t = &minefield->tiles[y][x];
        if (!t->mine) {
            t->mine = true;
            //t->visible = true; /* TODO remove this */
            i++;
        }
    }

    generate_surrounding(minefield); /* set the surrounding value in each tile */

    print_minefield(minefield);
    refresh();

    bool keeprunning = true;
    int ch;
    int cur_r, cur_c;
    Tile *cur_tile = NULL;
    while (keeprunning) {
        ch = getch(); /* wait for a character press */
        switch (ch) {
            case 'q': /* quit */
                keeprunning = false;
                break;

            /* movement keys */
            case 'h':
            case KEY_LEFT:
                if (minefield->cur.col > 0)
                    minefield->cur.col--;
                break;
            case 'j':
            case KEY_DOWN:
                if (minefield->cur.row < minefield->rows - 1)
                    minefield->cur.row++;
                break;
            case 'k':
            case KEY_UP:
                if (minefield->cur.row > 0)
                    minefield->cur.row--;
                break;
            case 'l':
            case KEY_RIGHT:
                if (minefield->cur.col < minefield->cols - 1)
                    minefield->cur.col++;
                break;

            case ' ':
                if (!reveal_tile(minefield, minefield->cur.row, minefield->cur.col))
                    death(minefield);
                break;

            case 'f':
                cur_r = minefield->cur.row;
                cur_c = minefield->cur.col;
                cur_tile = &minefield->tiles[cur_r][cur_c];
                if (!cur_tile->visible)
                    cur_tile->flagged = !cur_tile->flagged;
        }

        clear();
        print_minefield(minefield);
        refresh();
    }

    endwin();
    free(minefield);
}
