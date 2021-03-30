/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include <ncurses.h>
#include "minefield.h"

void draw_cursor(WINDOW *, Tile *, enum States);
void draw_tile(WINDOW *, Tile *, bool, bool, bool);
void draw_minefield(WINDOW *, Minefield *, bool, bool, enum States);
void draw_scoreboard(WINDOW *, Minefield *, int, enum States);
