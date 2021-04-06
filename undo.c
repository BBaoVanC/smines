/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include "config.h"

#if ALLOW_UNDO

#include <string.h>

#include "global.h"

/* copy_undo - copy the current game to the undo buffer */
void copy_undo() {
    undo_minefield = *minefield;
    undo_game_state = game_state;
}

/* undo - restore the game from the undo buffer
 * Note: pretty much just swaps the current game and what's stored in the undo
 * buffer, meaning that a second undo will "undo the undo" */
void undo() {
    Minefield buffer_minefield = *minefield;
    Game_State buffer_game_state = game_state;
    Coordinate buffer_cursor = minefield->cur;

    memcpy(minefield, &undo_minefield, sizeof(Minefield));
    minefield->cur = buffer_cursor;
    game_state = undo_game_state;

    undo_minefield = buffer_minefield;
    undo_game_state = buffer_game_state;
}

#endif
