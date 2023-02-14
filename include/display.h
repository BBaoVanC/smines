#ifndef SMINES_DRAW_H
#define SMINES_DRAW_H

#include "game.h"
#include "minefield.h"

#include <ncurses.h>

enum DisplayState {
    GAME, // showing the minesweeper game
    HELP, // help menu
};
struct Display {
    bool too_small;
    enum DisplayState state; // current screen we are displaying
    WINDOW *scoreboard;
    WINDOW *minefield;
    WINDOW *too_small_popup;

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
// remember to refresh manually
void display_draw(struct Display *display, struct Game *game);
// draw the entire minefield
// TODO: damage/regional update
void display_refresh(struct Display *display);

#endif
