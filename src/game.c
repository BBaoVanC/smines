#include "game.h"

void game_undo_store(struct Game *game) {
    game->undo.minefield = *game->minefield;
    game->undo.state = game->state;
}

void game_undo(struct Game *game) {
    enum GameState state_temp = game->state;
    struct Minefield minefield_temp = *game->minefield;
    game->state = game->undo.state;
    *game->minefield = game->undo.minefield;
    game->undo.state = state_temp;
    game->undo.minefield = minefield_temp;
}
