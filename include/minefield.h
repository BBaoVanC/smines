#ifndef SMINES_MINEFIELD_H
#define SMINES_MINEFIELD_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct Tile {
    bool mine;
    bool visible;
    bool flagged;
    uint8_t surrounding; /* if mine, this value is undefined! */
};

/* TODO: size_t or int/uint? */
struct Minefield {
    size_t width;
    size_t height;
    size_t mines;
    size_t placed_flags;
    struct {
        int x;
        int y;
    } cur;
    struct Tile *tiles;
};

/* does not populate mines, remember to run minefield_populate! */
/* also remember to run minefield_cleanup afterwards; it frees the tiles array */
/* */
/* if this returns false, then the tiles allocation failed! (and errno was likely set by calloc) */
bool minefield_init(struct Minefield *minefield, size_t width, size_t height, size_t mines);
void minefield_cleanup(struct Minefield *minefield);
void minefield_populate(struct Minefield *minefield);
struct Tile *minefield_get_tile(struct Minefield *minefield, size_t x, size_t y);
/* output: bool - false if the clicked tile was a mine, true otherwise */
bool minefield_reveal_tile(struct Minefield *minefield, size_t x, size_t y);
/* get how many mines are surrounding a tile */
size_t minefield_count_surrounding_mines(struct Minefield *minefield, size_t x, size_t y);
/* get how many flags are surrounding a tile */
size_t minefield_count_surrounding_flags(struct Minefield *minefield, size_t x, size_t y);

/* check if the game has been won yet */
bool minefield_check_victory(struct Minefield *);

#endif
