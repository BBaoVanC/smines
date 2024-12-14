#ifndef SMINES_GAME_H
#define SMINES_GAME_H

#include "minefield.h"

#include <stddef.h>
#include <stdint.h>

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

void game_init(struct Game *game, size_t width, size_t height, size_t mines);
void game_cleanup(struct Game *game);
void game_click_tile(struct Game *game, size_t x, size_t y);
void game_undo_store(struct Game *game);
void game_undo(struct Game *game);

#endif
