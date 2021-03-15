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
int get_surround_color(int);
bool reveal_tile(Minefield *, int, int);
void reveal_mines(Minefield *);
int get_surround(Minefield *, int, int);
int get_flag_surround(Minefield *, int, int);

bool check_victory(Minefield *);
