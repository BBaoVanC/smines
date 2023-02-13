/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#ifndef SMINES_WINDOW_H
#define SMINES_WINDOW_H

#include <ncurses.h>

/* Remove a window and any remnants
 * See https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/windows.html */
void destroy_win(WINDOW *);

#endif
