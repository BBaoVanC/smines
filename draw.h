/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include <ncurses.h>
#include "minefield.h"

void draw_tile(WINDOW *, Tile *, bool, bool);
void draw_minefield(WINDOW *, Minefield *, bool);
void draw_scoreboard(WINDOW *, Minefield *, int, int);
