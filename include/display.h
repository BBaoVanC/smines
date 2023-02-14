#ifndef SMINES_DRAW_H
#define SMINES_DRAW_H

#include "game.h"
#include "minefield.h"

#include <ncurses.h>

enum DisplayState {
    GAME, // showing the minesweeper game
    HELP, // help menu
    TOO_SMALL, // error that the screen is too small
};
struct Display {
    enum DisplayState state; // current screen we are displaying
    WINDOW *scoreboard;
    WINDOW *minefield;

    struct {
        int x, y;
    } origin;

    int min_rows, min_cols;
};

// should only be called once; there should only be one Display!
// needs to be cleaned up with display_destroy
void display_resize(struct Display *display, int minefield_rows, int minefield_cols);
// returns bool, true if successful. if false, terminate the entire program
bool display_init(struct Display *display, int minefield_rows, int minefield_cols);
void display_destroy(struct Display *display);
void display_draw(struct Display *display, struct Game *game);
// draw tile at specific row & col in the minefield
//void display_draw_tile(struct Display *display, struct Tile *tile, int row, int col);
// draw the entire minefield
// TODO: damage/regional update
//void display_draw_minefield(struct Display *display, struct Minefield *minefield);
//void display_draw_scoreboard(struct Display *display, struct Game *game);

#endif
