#include "minefield.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

bool minefield_init(struct Minefield *minefield, size_t width, size_t height, size_t mines) {
    minefield->width = width;
    minefield->height = height;
    minefield->mines = mines;
    minefield->placed_flags = 0;

    minefield->cur.x = width / 2;
    minefield->cur.y = height / 2;

    if (minefield->tiles != NULL) {
        free(minefield->tiles);
    }

    minefield->tiles = calloc(width * height, sizeof(struct Tile));
    if (!minefield->tiles) {
        return false;
    }

    return true;
}

void minefield_cleanup(struct Minefield *minefield) {
    free(minefield->tiles);
}

// set a tile as a mine and increment surrounding
static void minefield_set_mine(struct Minefield *minefield, size_t x, size_t y) {
    struct Tile *tile = minefield_get_tile(minefield, x, y);
    tile->mine = true;

    size_t x_start = x > 0 ? x - 1 : 0;
    size_t y_start = y > 0 ? y - 1 : 0;
    // TODO: this is kinda ugly
    size_t x_end = x < minefield->width - 1 ? x + 1 : x;
    size_t y_end = y < minefield->height - 1 ? y + 1 : y;
    for (size_t x1 = x_start; x1 <= x_end; x1++) {
        for (size_t y1 = y_start; y1 <= y_end; y1++) {
            minefield_get_tile(minefield, x1, y1)->surrounding++;
        }
    }
}
void minefield_populate(struct Minefield *minefield) {
    // randomly spread mines
    for (size_t i = 0; i < minefield->mines;) {
        // non inclusive; don't worry, i didn't forget about starting at 0
        size_t x = (rand() % (minefield->width));
        size_t y = (rand() % (minefield->height));

        // TODO: maybe calculate using distance formula
        // don't generate mines in a 3x3 centered on the cursor
        if ((x >= minefield->cur.x - 1) &&
            (y >= minefield->cur.y - 1) &&
            (x <= minefield->cur.x + 1) &&
            (y <= minefield->cur.y + 1)) {
            continue;
        }
        if (minefield_get_tile(minefield, x, y)->mine) {
            continue;
        }

        minefield_set_mine(minefield, x, y);
        i++;
    }
}

struct Tile *minefield_get_tile(struct Minefield *minefield, size_t x, size_t y) {
    // tile array is treated as a sequential list of rows, each row containing `minefield.cols` elements
    size_t offset = y * minefield->width;
    return &minefield->tiles[offset + x];
}

// output: bool - false if the clicked tile was a mine, true otherwise
bool minefield_reveal_tile(struct Minefield *minefield, size_t x, size_t y) {
    struct Tile *tile = minefield_get_tile(minefield, x, y);
    assert(!tile->flagged);
    bool start_visible = tile->visible;
    if (tile->mine) {
        return false;
    }
    tile->visible = true;
    if (tile->surrounding != 0 && !start_visible) {
        return true;
    }

    size_t x_start = x > 0 ? x - 1 : 0;
    size_t y_start = y > 0 ? y - 1 : 0;
    // TODO: this is kinda ugly
    size_t x_end = x < minefield->width - 1 ? x + 1 : x;
    size_t y_end = y < minefield->height - 1 ? y + 1 : y;
    bool no_mines = true;
    for (size_t x1 = x_start; x1 <= x_end; x1++) {
        for (size_t y1 = y_start; y1 <= y_end; y1++) {
            struct Tile *surtile = minefield_get_tile(minefield, x1, y1);
            if (!surtile->visible && !surtile->flagged) {
                no_mines &= minefield_reveal_tile(minefield, x1, y1);
            }
        }
    }
    return no_mines;
}

size_t minefield_count_surrounding_mines(struct Minefield *minefield, size_t x, size_t y) {
    size_t surrounding = 0;

    for (int x1 = x - 1; x1 <= x + 1; x1++) {
        for (int y1 = y - 1; y1 <= y + 1; y1++) {
            if ((x1 >= 0 && y1 >= 0) && (x1 < minefield->width && y1 < minefield->height)) { // make sure we are in bounds
                if (minefield_get_tile(minefield, x1, y1)->mine) {
                    surrounding++;
                }
            }
        }
    }

    return surrounding;
}

size_t minefield_count_surrounding_flags(struct Minefield *minefield, size_t x, size_t y) {
    size_t surrounding = 0;

    for (int x1 = x - 1; x1 <= x + 1; x1++) {
        for (int y1 = y - 1; y1 <= y + 1; y1++) {
            if ((x1 >= 0 && y1 >= 0) && (x1 < minefield->width && y1 < minefield->height)) { // make sure we are in bounds
                if (minefield_get_tile(minefield, x1, y1)->flagged) {
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
    size_t hidden = 0;
    for (size_t x = 0; x < minefield->width; x++) {
        for (size_t y = 0; y < minefield->height; y++) {
            if (!minefield_get_tile(minefield, x, y)->visible)
                hidden++;
        }
    }

    return hidden == minefield->mines;
}
