#include "game.h"

#include "minefield.h"

#include <stddef.h>

void game_init(struct Game *game, size_t width, size_t height, size_t mines) {
    game->state = ALIVE;
    minefield_init(&game->minefield, width, height, mines);
}

void game_cleanup(struct Game *game) {
    minefield_cleanup(&game->minefield);
}

void game_click_tile(struct Game *game, size_t x, size_t y) {
    game_undo_store(game);
    bool still_alive = minefield_reveal_tile(&game->minefield, x, y); /* false if dead from clicking a mine */
    if (!still_alive) {
        game->state = DEAD;
        /* reveal all the mines */
        size_t x, y;
        for (x = 0; x < game->minefield.width; x++) {
            for (y = 0; y < game->minefield.height; y++) {
                if (minefield_get_tile(&game->minefield, x, y)->mine) {
                    minefield_get_tile(&game->minefield, x, y)->visible = true;
                }
            }
        }
    } else if (minefield_check_victory(&game->minefield)) {
        game->state = VICTORY;
        size_t x, y;
        for (x = 0; x < game->minefield.width; x++) {
            for (y = 0; y < game->minefield.height; y++) {
                minefield_get_tile(&game->minefield, x, y)->visible = true;
            }
        }
    } else {
        /* TODO: try recursion or someting here */
    }
}

void game_undo_store(struct Game *game) {
    game->undo.minefield = game->minefield;
    game->undo.state = game->state;
}

void game_undo(struct Game *game) {
    enum GameState state_temp = game->state;
    struct Minefield minefield_temp = game->minefield;
    game->state = game->undo.state;
    game->minefield = game->undo.minefield;
    game->undo.state = state_temp;
    game->undo.minefield = minefield_temp;
}
