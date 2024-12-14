#ifndef SMINES_MINEFIELD_H
#define SMINES_MINEFIELD_H

#include <stdbool.h>
#include <stddef.h>

struct Tile {
    bool mine;
    bool visible;
    bool flagged;
    char surrounding; /* if mine, this value is undefined! */
};

/* TODO: int or int/uint? */
struct Minefield {
    int width;
    int height;
    int mines;
    int placed_flags;
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
bool minefield_init(struct Minefield *minefield, int width, int height, int mines);
void minefield_cleanup(struct Minefield *minefield);
void minefield_populate(struct Minefield *minefield);
struct Tile *minefield_get_tile(struct Minefield *minefield, int x, int y);
/* output: bool - false if the clicked tile was a mine, true otherwise */
bool minefield_reveal_tile(struct Minefield *minefield, int x, int y);
/* get how many mines are surrounding a tile */
int minefield_count_surrounding_mines(struct Minefield *minefield, int x, int y);
/* get how many flags are surrounding a tile */
int minefield_count_surrounding_flags(struct Minefield *minefield, int x, int y);

/* check if the game has been won yet */
bool minefield_check_victory(struct Minefield *);

#endif
