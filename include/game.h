#ifndef SMINES_GAME_H
#define SMINES_GAME_H

#include "minefield.h"

#include <stddef.h>

enum GameState {
    ALIVE, /* game is in progress */
    VICTORY, /* game has been won */
    DEAD, /* game was lost */
};
struct Game {
    enum GameState state;
    struct Minefield minefield;
    struct {
        enum GameState state;
        /* TODO: deep copy tiles array */
        struct Minefield minefield;
    } undo;
};

void game_init(struct Game *game, int width, int height, int mines);
void game_cleanup(struct Game *game);
void game_click_tile(struct Game *game, int x, int y);
void game_undo_store(struct Game *game);
void game_undo(struct Game *game);

#endif
