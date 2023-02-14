#ifndef SMINES_GAME_H
#define SMINES_GAME_H

#include "minefield.h"

#include <ncurses.h>

#include <stdint.h>

enum GameState {
    ALIVE, // game is in progress
    VICTORY, // game has been won
    DEAD, // game was lost
};
struct Undo {
    enum GameState state;
    struct Minefield minefield;
};
struct Game {
    enum GameState state;
    struct Minefield *minefield;
    uint32_t game_number;
    struct Undo undo;
};

void game_undo_store(struct Game *game);
void game_undo(struct Game *game);

#endif
