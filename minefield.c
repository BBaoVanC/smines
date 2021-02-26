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
    Minefield *minefield = (Minefield*)calloc(1, sizeof(Minefield));

    minefield->rows = rows;
    minefield->cols = cols;
    minefield->mines = mines;

    minefield->cur.col = cols / 2;
    minefield->cur.row = rows / 2;

    return minefield;
}

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
    } else if ((surrounding <= 8) && (surrounding >= 1)) { /* 1 <= surrounding <= 8 */
        return COLOR_PAIR(surrounding);
    } else {
        return COLOR_PAIR(100);
    }
}

void print_tile(Tile *tile, bool check_flag) {
    if (tile->flagged) {
        if (check_flag) {
            if (tile->mine) {
                attron(COLOR_PAIR(12));
                printw(" F");
                attroff(COLOR_PAIR(12));
            } else {
                attron(COLOR_PAIR(9));
                printw("!F");
                attroff(COLOR_PAIR(9));
            }
        } else {
            attron(COLOR_PAIR(12));
            printw(" F");
            attroff(COLOR_PAIR(12));
        }

    } else if (tile->visible) {
        if (tile->mine) {
            attron(COLOR_PAIR(9));
            printw(" X");
            attroff(COLOR_PAIR(9));
        } else {
            int color = getcolorforsurround(tile->surrounding);
            attron(color);
            if (tile->surrounding == 0)
                printw("  ", tile->surrounding);
            else
                printw(" %d", tile->surrounding);
            attroff(color);
        }

    } else {
        attron(COLOR_PAIR(11));
        printw("  ");
        attroff(COLOR_PAIR(11));
    }
}

void print_cursor_tile(Tile *tile) {
    if (tile->flagged) {
        attron(COLOR_PAIR(13));
        printw(" F");
        attroff(COLOR_PAIR(13));
    } else if (tile->visible) {
        if (tile->mine) {
            attron(COLOR_PAIR(13));
            printw(" X");
            attroff(COLOR_PAIR(13));
        } else {
            attron(COLOR_PAIR(13));
            if (tile->surrounding == 0)
                printw("  ", tile->surrounding);
            else
                printw(" %d", tile->surrounding);
            attroff(COLOR_PAIR(13));
        }
    } else {
        attron(COLOR_PAIR(13));
        printw("  ");
        attroff(COLOR_PAIR(13));
    }
}

void print_minefield(Minefield *minefield, bool check_flag) {
    int cur_r = minefield->cur.row;
    int cur_c = minefield->cur.col;

    for (int y = 0; y < minefield->rows; y++) {
        for (int x = 0; x < minefield->cols; x++) {
            move(y, x*2);
            print_tile(&minefield->tiles[y][x], check_flag);
            printw("\n");
        }
    }

    move(cur_r, cur_c*2);
    print_cursor_tile(&minefield->tiles[cur_r][cur_c]);
}

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
                if ((r >= 0) && (c >= 0) && (r < minefield->rows) && (c < minefield->cols)) {
                    if (!minefield->tiles[r][c].visible) {
                        if (minefield->tiles[r][c].surrounding == 0 || 1) {
                            reveal_tile(minefield, r, c);
                        }
                    }
                }
            }
        }
        return true;
    }
}

void reveal_mines(Minefield *minefield) {
    for (int r = 0; r < minefield->rows; r++) {
        for (int c = 0; c < minefield->cols; c++) {
            if (minefield->tiles[r][c].mine) {
                minefield->tiles[r][c].visible = true;
            }
        }
    }
}

int getsurround(Minefield *minefield, int row, int col) {
    int r, c;
    int surrounding = 0;
    Tile *current_tile = NULL;

    for (r = row - 1; r < row + 2; r++) {
        for (c = col - 1; c < col + 2; c++) {
            if ((r >= 0 && c >= 0) && (r < minefield->rows && c < minefield->cols)) {
                current_tile = &minefield->tiles[r][c];
                if (current_tile->mine) {
                    surrounding++;
                }
            }
        }
    }

    return surrounding;
}
