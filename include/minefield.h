/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#ifndef SMINES_MINEFIELD_H
#define SMINES_MINEFIELD_H

#include "types.h"

#include <ncurses.h>
#include <stdbool.h>

struct Tile {
    bool mine;
    bool visible;
    bool flagged;
    size_t surrounding;
};

// TODO: size_t or int/uint?
struct Minefield {
    size_t rows;
    size_t cols;
    size_t mines;
    size_t placed_flags;
    struct cur {
        size_t col;
        size_t row;
    };
    struct Tile *tiles;
};

void minefield_init(struct Minefield *minefield, size_t rows, size_t cols, size_t mines);
struct Tile *minefield_get_tile(struct Minefield *minefield, size_t row, size_t col);
/* Figure out what color to use based on surrounding tiles */
int get_surround_color(int surrounding);
// output: bool - false if the clicked tile was a mine, true otherwise
bool minefield_reveal_tile(struct Minefield *, size_t row, size_t col);
/* Find every mine and make it visible */
void reveal_mines(struct Minefield *);
/* Count surrounding mines of a single tile */
size_t minefield_count_surrounding_mines(struct Minefield *minefield, size_t row, size_t col);
/* Count the surrounding flags, this is get_surround but with flags instead */
size_t minefield_count_surrounding_mines(struct Minefield *minefield, size_t row, size_t col);

/* Check if the player won yet */
bool minefield_check_victory(struct Minefield *);

#endif
