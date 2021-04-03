/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include "config.h"

#include "help.h"
#include <ncurses.h>

const char helptxt[] =
    "H or ?: view this help page\n"
    "L: redraw screen (just in case)\n"
    "\n"
    "q: quit\n"
    "r: new game\n"
    "space: reveal tile under cursor\n"
    "f: place flag\n"
#if ALLOW_UNDO
    "u: undo last move (undoing a second time will 'undo the undo')\n"
#endif
    "\n"
    "Use hjkl or arrow keys to move\n"
    "0 or ^: jump to left side\n"
    "$: jump to right side\n"
    "g: jump to top side\n"
    "G: jump to bottom side\n"
    ;

void draw_help(WINDOW *win) {
    wmove(win, 0, 0);
    wprintw(win, helptxt);
}
