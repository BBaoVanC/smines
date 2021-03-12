/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include <ncurses.h>
#include "types.h"

#ifndef __SMINES_GLOBAL_H
#define __SMINES_GLOBAL_H


const int SCOREBOARD_ROWS = 4;
Minefield *minefield = NULL;
WINDOW *fieldwin = NULL;
WINDOW *scorewin = NULL;
int origin_x, origin_y;
int game_number = 0; /* start at 0 because it's incremented before each game */
bool screen_too_small = FALSE;

#endif
