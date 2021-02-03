/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "minefield.h"
#include "colors.h"

Tile *get_tile_ptr(Minefield *minefield, int col, int row) {
    /* https://www.tutorialspoint.com/how-to-dynamically-allocate-a-2d-array-in-c */
    return (minefield->tiles + col*row + row);
}

void init_minefield(Minefield *minefield, int cols, int rows) {
    printf("Setting cols to %i\n", cols);
    minefield->cols = cols;
    printf("Setting rows to %i\n", rows);
    minefield->rows = rows;
    printf("Setting mines to 0\n");
    minefield->mines = 0;

    /* https://www.tutorialspoint.com/how-to-dynamically-allocate-a-2d-array-in-c */
    printf("Allocating tiles array\n");
    minefield->tiles = (Tile *)malloc(cols * rows * sizeof(Tile));
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
            int color = getcolorforsurround(tile->surrounding); // TODO
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
            print_tile(get_tile_ptr(minefield, x, y));
        }
    }
}
