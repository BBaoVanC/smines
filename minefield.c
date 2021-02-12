/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "minefield.h"
#include "config.h"

Minefield *init_minefield(int rows, int cols, int mines) {
    /* Allocate memory for minefield */
    Minefield *minefield = malloc(sizeof(Minefield));

    minefield->rows = rows;
    minefield->cols = cols;
    minefield->mines = mines;

    return minefield;
}

void generate_surrounding(Minefield *minefield) {
    Tile *t = NULL;
    for (int c = 0; c < minefield->cols; c++) {
        for (int r = 0; r < minefield->rows; r++) {
            t = &minefield->tiles[r][c];
            t->surrounding = getsurround(minefield, r, c);
        }
    }
}

int getcolorforsurround(int surrounding) {
    if (surrounding == 0) {
        return COLOR_PAIR(10);
    } else if ((surrounding <= 8) && (surrounding >= 1)) {
        return COLOR_PAIR(surrounding);
    } else {
        return COLOR_PAIR(100);
    }
}

void print_tile(Tile *tile) {
    if (tile->flagged) {
        attron(COLOR_PAIR(12));
        printw(" F");
        attroff(COLOR_PAIR(12));
    } else if (tile->visible) {
        if (tile->mine) {
            attron(COLOR_PAIR(9));
            printw(" X");
            attroff(COLOR_PAIR(9));
        } else {
            int color = getcolorforsurround(tile->surrounding);
            attron(color);
            printw(" %d", tile->surrounding);
            attroff(color);
        }
    } else {
        attron(COLOR_PAIR(11));
        printw("  ");
        attroff(COLOR_PAIR(11));
    }
}

void print_minefield(Minefield *minefield) {
    for (int y = 0; y < minefield->rows; y++) {
        for (int x = 0; x < minefield->cols; x++) {
            move(y, x*2);
            print_tile(&minefield->tiles[y][x]);
            printw("\n");
        }
    }
}

int getsurround(Minefield *minefield, int row, int col) {
    int r, c;
    int surrounding = 0;
    Tile *current_tile = NULL;
#if 0
    int rstart = 0;
    int rend = 0;
    int cstart = 0;
    int cend = 0;

    if (row > 0)
        rstart = row - 1;
    if (row < minefield->rows - 1)
        rend = row + 1;

    if (col > 0)
        cstart = col - 1;
    if (col < minefield->rows - 1)
        cend = col + 1;
#endif

    for (r = row - 1; r < row + 1; r++) {
        for (c = col - 1; c < col + 1; c++) {
            if (r >= 0 && c >= 0) {
                current_tile = &minefield->tiles[r][c];
                if (current_tile->mine) {
                    surrounding++;
                }
            }
        }
    }

    return surrounding;
}
