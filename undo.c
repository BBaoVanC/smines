/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include "config.h"

#if ALLOW_UNDO

#include <string.h>

#include "global.h"

void copy_undo() {
    undo_minefield = *minefield;
    undo_game_state = game_state;
}

void undo() {
    Minefield buffer_minefield;
    enum States buffer_game_state;

    buffer_minefield = *minefield;
    buffer_game_state = game_state;

    memcpy(minefield, &undo_minefield, sizeof(Minefield));
    game_state = undo_game_state;

    undo_minefield = buffer_minefield;
    undo_game_state = buffer_game_state;
}

#endif
