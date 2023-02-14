#include "minefield.h"

#include <assert.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

void minefield_init(struct Minefield *minefield, size_t rows, size_t cols, size_t mines) {
    minefield->rows = rows;
    minefield->cols = cols;
    minefield->mines = mines;
    minefield->placed_flags = 0;

    minefield->cur.col = cols / 2;
    minefield->cur.row = rows / 2;

    // TODO: should we take this as an arg instead of allocating?
    minefield->tiles = calloc(rows * cols, sizeof(struct Tile));

    for (size_t i = 0; i < rows * cols; i++) {
        minefield->tiles[i].visible = true;
    }
}

// set a tile as a mine and increment surrounding
static void minefield_set_mine(struct Minefield *minefield, size_t row, size_t col) {
    struct Tile *tile = minefield_get_tile(minefield, row, col);
    assert(tile->mine == false);
    tile->mine = true;

    // int because otherwise subtraction might overflow
    for (int r = row - 1; r <= row + 1; r++) {
        for (int c = col - 1; c <= col + 1; c++) {
            // bounds check to prevent infinite recursion
            if (c >= 0 && r >= 0 && c < minefield->cols && r < minefield->rows) {
                minefield_get_tile(minefield, r, c)->surrounding++;
            }
        }
    }
}
void minefield_populate(struct Minefield *minefield) {
    // randomly spread mines
    int r, c; // otherwise our 3x3 check might overflow and break
    size_t i = 0;
    while (i < minefield->mines) {
        r = (rand() % (minefield->rows)); // non inclusive; i didn't forget about starting at 0
        c = (rand() % (minefield->cols));

        // don't generate mines in a 3x3 centered on the cursor
        if ((r >= minefield->cur.row - 1) &&
            (c >= minefield->cur.col - 1) &&
            (r <= minefield->cur.row + 1) &&
            (c <= minefield->cur.col + 1)) {
            continue;
        }
        if (minefield_get_tile(minefield, r, c)->mine) {
            continue;
        }

        minefield_set_mine(minefield, r, c);
        i++;
    }
}

void minefield_cleanup(struct Minefield *minefield) {
    free(minefield->tiles);
}

struct Tile *minefield_get_tile(struct Minefield *minefield, size_t row, size_t col) {
    // tile array is treated as a sequential list of rows, each row containing `minefield.cols` elements
    size_t row_start = row * minefield->cols; // index of start of row
    return &minefield->tiles[row_start + col];
}

// output: bool - false if the clicked tile was a mine, true otherwise
bool minefield_reveal_tile(struct Minefield *minefield, size_t row, size_t col) {
    struct Tile *tile = minefield_get_tile(minefield, row, col);
    if (tile->mine) {
        return false;
    } else {
        tile->visible = true;
        size_t r, c;
        if (tile->surrounding != 0)
            return true;

        for (r = row - 1; r < row + 2; r++) {
            for (c = col - 1; c < col + 2; c++) {
                if ((r >= 0) && (c >= 0) && (r < minefield->rows) && (c < minefield->cols)) { // stay in bounds
                    if (!minefield_get_tile(minefield, r, c)->visible) {
                        minefield_reveal_tile(minefield, r, c);
                    }
                }
            }
        }
        return true;
    }
}

size_t minefield_count_surrounding_mines(struct Minefield *minefield, size_t row, size_t col) {
    size_t r, c;
    size_t surrounding = 0;
    struct Tile *current_tile = NULL;

    for (r = row - 1; r < row + 2; r++) {
        for (c = col - 1; c < col + 2; c++) {
            if ((r >= 0 && c >= 0) && (r < minefield->rows && c < minefield->cols)) { // make sure we are in bounds
                current_tile = minefield_get_tile(minefield, r, c);
                if (current_tile->mine) {
                    surrounding++;
                }
            }
        }
    }

    return surrounding;
}

size_t minefield_count_surrounding_flags(struct Minefield *minefield, size_t row, size_t col) {
    size_t r, c;
    size_t surrounding = 0;
    struct Tile *current_tile = NULL;

    for (r = row - 1; r < row + 2; r++) {
        for (c = col - 1; c < col + 2; c++) {
            if ((r >= 0 && c >= 0) && (r < minefield->rows && c < minefield->cols)) { // make sure we are in bounds
                current_tile = minefield_get_tile(minefield, r, c);
                if (current_tile->flagged) {
                    surrounding++;
                }
            }
        }
    }

    return surrounding;
}

bool minefield_check_victory(struct Minefield *minefield) {
    /* TODO: count up the hidden tiles as they are revealed so they
     * don't have to be recounted every time this function runs */
    size_t r, c;
    size_t hidden = 0;
    for (r = 0; r < minefield->rows; r++) {
        for (c = 0; c < minefield->cols; c++) {
            if (!minefield_get_tile(minefield, r, c)->visible)
                hidden++;
        }
    }

    return hidden == minefield->mines;
}
