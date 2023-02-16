#include "game.h"

#include "minefield.h"

void game_click_tile(struct Game *game, size_t row, size_t col) {
    game_undo_store(game);
    bool still_alive = minefield_reveal_tile(&game->minefield, row, col); // false if dead from clicking a mine
    if (!still_alive) {
        game->state = DEAD;
        // reveal all the mines
        for (size_t r = 0; r < game->minefield.rows; r++) {
            for (size_t c = 0; c < game->minefield.cols; c++) {
                if (minefield_get_tile(&game->minefield, r, c)->mine) {
                    minefield_get_tile(&game->minefield, r, c)->visible = true;
                }
            }
        }
    } else if (minefield_check_victory(&game->minefield)) {
        game->state = VICTORY;
        int r, c;
        for (r = 0; r < game->minefield.rows; r++) {
            for (c = 0; c < game->minefield.cols; c++) {
                minefield_get_tile(&game->minefield, r, c)->visible = true;
            }
        }
    } else {
        // TODO: try recursion or someting here
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
