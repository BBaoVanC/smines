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

Minefield *init_minefield(int rows, int cols, int mines) {
    /* Allocate memory for minefield */
    Minefield *minefield = (Minefield*)calloc(1, sizeof(Minefield));

    minefield->rows = rows;
    minefield->cols = cols;
    minefield->mines = mines;
    minefield->placed_flags = 0;

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
                        reveal_tile(minefield, r, c);
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

int getflagsurround(Minefield *minefield, int row, int col) {
    int r, c;
    int surrounding = 0;
    Tile *current_tile = NULL;

    for (r = row - 1; r < row + 2; r++) {
        for (c = col - 1; c < col + 2; c++) {
            if ((r >= 0 && c >= 0) && (r < minefield->rows && c < minefield->cols)) {
                current_tile = &minefield->tiles[r][c];
                if (current_tile->flagged) {
                    surrounding++;
                }
            }
        }
    }

    return surrounding;
}

bool check_victory(Minefield *minefield) {
    /* this function feels too inefficient */
    int r, c;
    int hidden = 0;
    for (r = 0; r < minefield->rows; r++) {
        for (c = 0; c < minefield->cols; c++) {
            if (!minefield->tiles[r][c].visible)
                hidden++;
        }
    }

    if (hidden == minefield->mines)
        return true;
    else
        return false;
}

bool victory(Minefield *minefield, WINDOW *fieldwin, WINDOW *scorewin, int game_number) {
    int r, c;
    for (r = 0; r < minefield->rows; r++) {
        for (c = 0; c < minefield->cols; c++) {
            minefield->tiles[r][c].visible = true;
        }
    }

    draw_minefield(fieldwin, minefield, true);
    wborder(fieldwin, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(fieldwin);

    draw_scoreboard(scorewin, minefield, game_number);
    wrefresh(scorewin);

    wattron(scorewin, A_BOLD);
    wattron(scorewin, COLOR_PAIR(MSG_WIN));
    mvwprintw(scorewin, 0, 0, "YOU WIN!");
    wattroff(scorewin, COLOR_PAIR(MSG_WIN));
    wattroff(scorewin, A_BOLD);
    wrefresh(scorewin);

    int ch;
    while (true) { /* wait for either 'q' to quit or 'r' to restart */
        ch = getch();
        switch (ch) {
            case 'r':
                return true;
                break;
            case 'q':
                return false;
                break;
        }
    }
}

bool death(Minefield *minefield, WINDOW *fieldwin, WINDOW *scorewin, int game_number) {
    reveal_mines(minefield);

    draw_minefield(fieldwin, minefield, true);
    wborder(fieldwin, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(fieldwin);

    draw_scoreboard(scorewin, minefield, game_number);
    wrefresh(scorewin);

    wattron(scorewin, A_BOLD);
    wattron(scorewin, COLOR_PAIR(MSG_DEATH));
    mvwprintw(scorewin, 0, 0, "YOU DIED!");
    wattroff(scorewin, COLOR_PAIR(MSG_DEATH));
    wattroff(scorewin, A_BOLD);
    wrefresh(scorewin);

    int c;
    while (true) { /* wait for either 'q' to quit or 'r' to restart */
        c = getch();
        switch (c) {
            case 'r':
                return true;
                break;
            case 'q':
                return false;
                break;
        }
    }
}
