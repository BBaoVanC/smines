/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#ifndef SMINES_MINEFIELD_H
#define SMINES_MINEFIELD_H

#include "config.h"
#include "types.h"

#include <ncurses.h>
#include <stdbool.h>

/* Create a minefield
 * remember to populate_mines() since this won't do it automatically */
Minefield *init_minefield(int, int, int);
void populate_mines(Minefield *);
/* Calculate the surrounding mines for every single tile */
void generate_surrounding(Minefield *);
/* Figure out what color to use based on surrounding tiles */
int get_surround_color(int);
/* Returns a bool: false if dead */
bool reveal_tile(Minefield *, int, int);
/* Find every mine and make it visible */
void reveal_mines(Minefield *);
/* Count surrounding mines of a single tile */
int get_surround(Minefield *, int, int);
/* Count the surrounding flags, this is get_surround but with flags instead */
int get_flag_surround(Minefield *, int, int);

/* Check if the player won yet */
bool check_victory(Minefield *);

#endif
