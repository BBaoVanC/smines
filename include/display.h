#ifndef SMINES_DISPLAY_H
#define SMINES_DISPLAY_H

#include "game.h"
#include "minefield.h"

#include <ncurses.h>

enum DisplayState {
    GAME, // showing the minesweeper game
    HELP, // help menu
};
struct Display {
    bool too_small;
    bool erase_needed; // if entire screen needs to be erased (during transition)
    struct Game *game;
    uint32_t game_number;
    enum DisplayState state; // current screen we are displaying
    WINDOW *scoreboard;
    WINDOW *minefield;
    WINDOW *too_small_popup;

    struct {
        int x, y;
    } origin;

    int min_width, min_height;
};

// should only be called once; there should only be one Display!
// needs to be cleaned up with display_destroy
void display_resize(struct Display *display);
// returns bool, true if successful. if false, terminate the entire program
// not ready for use until you call display_set_game
bool display_init(struct Display *display);
void display_set_game(struct Display *display, struct Game *game);
void display_destroy(struct Display *display);
// remember to refresh manually
void display_draw(struct Display *display);
// draw the entire minefield
// TODO: damage/regional update
void display_refresh(struct Display *display);
// switch to help screen
void display_transition_help(struct Display *display);
// switch to main game screen
void display_transition_game(struct Display *display);

#endif
