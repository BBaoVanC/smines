/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "colornames.h"
#include "minefield.h"
#include "draw.h"
#include "types.h"
#include "config.h"

/* init_minefield - create a pointer to a new minefield
 * inputs:
 *  int rows: how many rows of tiles
 *  int cols: how many columns of tiles
 *  int mines: how many mines (remember to run populate_mines)
 * output:
 *  pointer to a new minefield instance
 */
Minefield *init_minefield(int rows, int cols, int mines) {
    /* uses calloc instead of malloc because calloc zeros the memory automatically */
    Minefield *minefield = (Minefield *)calloc(1, sizeof(Minefield));

    minefield->rows = rows;
    minefield->cols = cols;
    minefield->mines = mines;
    minefield->placed_flags = 0;

    minefield->cur.col = cols / 2;
    minefield->cur.row = rows / 2;

    return minefield;
}

/* populate_mines - randomly place mines throughout the minefield
 * inputs:
 *  Minefield *minefield: the minefield (amount of mines is read from here too)
 *  int excl_r and excl_c: the coordinate to make a zero tile
 *      It's tradition to make the starting position a zero tile since it's the
 *      most helpful start (getting a one as your first tile is pretty much just a luck game)
 */
void populate_mines(Minefield *minefield, int excl_r, int excl_c) {
    int i, r, c;
    Tile *t = NULL;
    for (i = 0; i < minefield->mines;) { /* don't increment i here because it's incremented later if it's an acceptable place */
        r = (rand() % (minefield->rows - 1 + 1)); /* generate random row */
        c = (rand() % (minefield->cols - 1 + 1)); /* generate random col */
        t = &minefield->tiles[r][c];

        /* the following abomination makes sure the 8 surrounding mines
         * around the cursor aren't mines
         */
        if (!((r >= minefield->cur.row - 1) &&
             (c >= minefield->cur.col - 1) &&
             (r <= minefield->cur.row + 1) &&
             (r <= minefield->cur.col + 1))) {
            if (!t->mine) { /* prevent overlapping of mines */
                if ((r != excl_r) && (c != excl_c)) {
                    t->mine = true;
                    i++; /* since it's not incremented by the for loop */
                }
            }
        }
    }
}


/* generate_surrounding - generate the surrounding value for all tiles in a minefield
 * inputs:
 *  Minefield *minefield: minefield containing the tiles array to use
 */
void generate_surrounding(Minefield *minefield) {
    Tile *t = NULL;
    for (int c = 0; c < minefield->cols; c++) {
        for (int r = 0; r < minefield->rows; r++) {
            t = &minefield->tiles[r][c];
            t->surrounding = get_surround(minefield, r, c);
        }
    }
}

/* get_surround_color - get the color pair to use based on amount of surrounding tiles
 * inputs:
 *  int surrounding: the amount of surrounding tiles
 */
int get_surround_color(int surrounding) {
    if (surrounding == 0) {
        return COLOR_PAIR(10);
    } else if ((surrounding <= 8) && (surrounding >= 1)) { /* 1 <= surrounding <= 8 */
        return COLOR_PAIR(surrounding);
    } else {
        return COLOR_PAIR(100);
    }
}

/* reveal_tile - (recursively) reveal a tile
 * inputs:
 *  Minefield *minefield: the minefield containing the tile
 *  int row: the row of the tile to reveal
 *  int col: the column of the tile to reveal
 * output:
 *  bool - false if the clicked tile was a mine, true otherwise
 */
bool reveal_tile(Minefield *minefield, int row, int col) {
    Tile *tile = &minefield->tiles[row][col];
    if (tile->mine) {
        return false;
    } else {
        tile->visible = true;
        int r, c;
        if (tile->surrounding != 0)
            return true;

        for (r = row - 1; r < row + 2; r++) {
            for (c = col - 1; c < col + 2; c++) {
                if ((r >= 0) && (c >= 0) && (r < minefield->rows) && (c < minefield->cols)) { /* stay in bounds */
                    if (!minefield->tiles[r][c].visible) {
                        reveal_tile(minefield, r, c);
                    }
                }
            }
        }
        return true;
    }
}

/* reveal_mines - reveal every single mine
 * inputs:
 *  Minefield *minefield: the minefield to reveal mines in
 */
void reveal_mines(Minefield *minefield) {
    for (int r = 0; r < minefield->rows; r++) {
        for (int c = 0; c < minefield->cols; c++) {
            if (minefield->tiles[r][c].mine) {
                minefield->tiles[r][c].visible = true;
            }
        }
    }
}

/* get_surround - count the surrounding tiles of a tile (3x3 check area)
 * Note: if the tile itself is a mine, then it's included in the count, but
 *       there shouldn't be any reason to need to see this value on a mine.
 * inputs:
 *  Minefield *minefield: needed so we can find the surrounding tiles
 *  int row: the row of the tile to check
 *  int col: the column of the tile to check
 * output:
 *  int - the amount of surrounding mines
 */
int get_surround(Minefield *minefield, int row, int col) {
    int r, c;
    int surrounding = 0;
    Tile *current_tile = NULL;

    for (r = row - 1; r < row + 2; r++) {
        for (c = col - 1; c < col + 2; c++) {
            if ((r >= 0 && c >= 0) && (r < minefield->rows && c < minefield->cols)) { /* make sure we are in bounds */
                current_tile = &minefield->tiles[r][c];
                if (current_tile->mine) {
                    surrounding++;
                }
            }
        }
    }

    return surrounding;
}

/* get_flag_surround - basically just get_surround but with flags instead of mines
 * inputs:
 *  Minefield *minefield: needed to read the surrounding tiles
 *  int row: the row of the tile to check
 *  int col: the column of the tile to check
 * output:
 *  int - the amount of surrounding flags
 */
int get_flag_surround(Minefield *minefield, int row, int col) {
    int r, c;
    int surrounding = 0;
    Tile *current_tile = NULL;

    for (r = row - 1; r < row + 2; r++) {
        for (c = col - 1; c < col + 2; c++) {
            if ((r >= 0 && c >= 0) && (r < minefield->rows && c < minefield->cols)) { /* make sure we are in bounds */
                current_tile = &minefield->tiles[r][c];
                if (current_tile->flagged) {
                    surrounding++;
                }
            }
        }
    }

    return surrounding;
}

/* check_victory - check if the player has won
 * inputs:
 *  Minefield *minefield
 * output:
 *  bool - true if the player has won, false otherwise
 */
bool check_victory(Minefield *minefield) {
    /* TODO: count up the hidden tiles as they are revealed so they
     * don't have to be recounted every time this function runs */
    int r, c;
    int hidden = 0;
    for (r = 0; r < minefield->rows; r++) {
        for (c = 0; c < minefield->cols; c++) {
            if (!minefield->tiles[r][c].visible)
                hidden++;
        }
    }

    return hidden == minefield->mines;
}
