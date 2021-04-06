/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include <ncurses.h>
#include "minefield.h"

void draw_tile_color(WINDOW *, Tile *, Game_State);
void draw_tile(WINDOW *, Tile *, int, Game_State);

void draw_minefield(WINDOW *, Minefield *, bool, bool, Game_State);
void draw_scoreboard(WINDOW *, Minefield *, int, Game_State);
