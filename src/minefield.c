#include "minefield.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

bool minefield_init(struct Minefield *minefield, int width, int height, int mines) {
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

/* set a tile as a mine and increment surrounding */
static void minefield_set_mine(struct Minefield *minefield, int x, int y) {
    struct Tile *tile = minefield_get_tile(minefield, x, y);
    tile->mine = true;

    int x_start = x > 0 ? x - 1 : 0;
    int y_start = y > 0 ? y - 1 : 0;
    /* TODO: this is kinda ugly */
    int x_end = x < minefield->width - 1 ? x + 1 : x;
    int y_end = y < minefield->height - 1 ? y + 1 : y;

    int x1, y1;
    for (x1 = x_start; x1 <= x_end; x1++) {
        for (y1 = y_start; y1 <= y_end; y1++) {
            minefield_get_tile(minefield, x1, y1)->surrounding++;
        }
    }
}
void minefield_populate(struct Minefield *minefield) {
    /* randomly spread mines */
    int i;
    for (i = 0; i < minefield->mines;) {
        /* non inclusive; don't worry, i didn't forget about starting at 0 */
        int x = (rand() % (minefield->width));
        int y = (rand() % (minefield->height));

        /* TODO: maybe calculate using distance formula */
        /* don't generate mines in a 3x3 centered on the cursor */
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

struct Tile *minefield_get_tile(struct Minefield *minefield, int x, int y) {
    /* tile array is treated as a sequential list of rows, each row containing `minefield.cols` elements */
    int offset = y * minefield->width;
    return &minefield->tiles[offset + x];
}

/* output: bool - false if the clicked tile was a mine, true otherwise */
bool minefield_reveal_tile(struct Minefield *minefield, int x, int y) {
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

    int x_start = x > 0 ? x - 1 : 0;
    int y_start = y > 0 ? y - 1 : 0;
    /* TODO: this is kinda ugly */
    int x_end = x < minefield->width - 1 ? x + 1 : x;
    int y_end = y < minefield->height - 1 ? y + 1 : y;
    bool no_mines = true;

    int x1, y1;
    for (x1 = x_start; x1 <= x_end; x1++) {
        for (y1 = y_start; y1 <= y_end; y1++) {
            struct Tile *surtile = minefield_get_tile(minefield, x1, y1);
            if (!surtile->visible && !surtile->flagged) {
                no_mines &= minefield_reveal_tile(minefield, x1, y1);
            }
        }
    }
    return no_mines;
}

int minefield_count_surrounding_mines(struct Minefield *minefield, int x, int y) {
    int surrounding = 0;

    int x1, y1;
    for (x1 = x - 1; x1 <= x + 1; x1++) {
        for (y1 = y - 1; y1 <= y + 1; y1++) {
            if ((x1 >= 0 && y1 >= 0) && (x1 < minefield->width && y1 < minefield->height)) { /* make sure we are in bounds */
                if (minefield_get_tile(minefield, x1, y1)->mine) {
                    surrounding++;
                }
            }
        }
    }

    return surrounding;
}

int minefield_count_surrounding_flags(struct Minefield *minefield, int x, int y) {
    int surrounding = 0;

    int x1, y1;
    for (x1 = x - 1; x1 <= x + 1; x1++) {
        for (y1 = y - 1; y1 <= y + 1; y1++) {
            if ((x1 >= 0 && y1 >= 0) && (x1 < minefield->width && y1 < minefield->height)) { /* make sure we are in bounds */
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
    int hidden = 0;
    int x, y;
    for (x = 0; x < minefield->width; x++) {
        for (y = 0; y < minefield->height; y++) {
            if (!minefield_get_tile(minefield, x, y)->visible)
                hidden++;
        }
    }

    return hidden == minefield->mines;
}
