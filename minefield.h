/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include <stdbool.h>
#include <ncurses.h>
#include "types.h"
#include "config.h"

Minefield *init_minefield(int, int, int);
void populate_mines(Minefield *, int, int);
void generate_surrounding(Minefield *);
int getcolorforsurround(int);
bool reveal_tile(Minefield *, int, int);
void reveal_mines(Minefield *);
int getsurround(Minefield *, int, int);
int getflagsurround(Minefield *, int, int);

bool check_victory(Minefield *);
bool victory(Minefield *, WINDOW *, WINDOW *, int);
bool death(Minefield *, WINDOW *, WINDOW *, int);
