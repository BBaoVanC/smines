/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#ifndef SMINES_DRAW_H
#define SMINES_DRAW_H

#include "minefield.h"

#include <ncurses.h>

/* draw_tile with color */
void draw_tile_color(WINDOW *, Tile *, Game_State);
void draw_tile(WINDOW *, Tile *, int, Game_State);

void draw_minefield(WINDOW *, Minefield *, Game_State);
void draw_scoreboard(WINDOW *, Minefield *, int, Game_State);

#endif
