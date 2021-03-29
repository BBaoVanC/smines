/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include <ncurses.h>
#include "minefield.h"
#include "types.h"

extern int SCOREBOARD_ROWS;
extern Minefield *minefield;
extern WINDOW *fieldwin;
extern WINDOW *scorewin;
extern int origin_x, origin_y;
extern int game_number; /* start at 0 because it's incremented before each game */
extern bool screen_too_small;
extern enum States game_state;

extern bool help_visible; /* if true, draw help page **instead of** everything else */
