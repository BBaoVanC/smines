#include "minefield.h"

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
}

void minefield_populate(struct Minefield *minefield) {
    // randomly spread mines
    size_t i, r, c;
    struct Tile *t = NULL;
    for (i = 0; i < minefield->mines;) {          // don't increment i here because it's incremented later if it's an acceptable place
        r = (rand() % (minefield->rows - 1 + 1)); // TODO: why did i do - 1 + 1
        c = (rand() % (minefield->cols - 1 + 1));
        t = minefield_get_tile(minefield, r, c);

        /* When I first checked this I thoguht it was too complicated
         * but reading it, now I realize that it is just checking the 3x3,
         * but it looks bad because there's two dimensions */
        if (!((r >= minefield->cur.row - 1) &&
              (c >= minefield->cur.col - 1) &&
              (r <= minefield->cur.row + 1) &&
              (c <= minefield->cur.col + 1))) {
            if (!t->mine) { // prevent overlapping of mines
                t->mine = true;
                i++; // since it's not incremented by the for loop
            }
        }
    }

    // generate surrounding mines count for each tile
    for (size_t y = 0; y < minefield->rows; y++) {
        for (size_t x = 0; x < minefield->cols; x++) {
            if (minefield_get_tile(minefield, y, x)->mine) {
                // if the tile is a mine, increment `surrounding` on the nearby tiles by one
                for (size_t r = y - 1; r <= y + 1; r++) {
                    for (size_t c = x - 1; c <= x + 1; c++) {
                        // do a bounds check, otherwise runaway recursion
                        if ((c >= 0) && (r >= 0) && (c < minefield->cols) && (r < minefield->rows)) {
                            minefield_get_tile(minefield, r, c)->surrounding++;
                        }
                    }
                }
            }
        }
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
