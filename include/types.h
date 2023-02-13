/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#ifndef SMINES_TYPES_H
#define SMINES_TYPES_H

#include <stdbool.h>

typedef struct {
    int col;
    int row;
} Coordinate;

typedef struct {
    bool mine;
    bool visible;
    bool flagged;
    int surrounding;
} Tile;

typedef struct {
    int rows;
    int cols;
    int mines;
    int placed_flags;
    Coordinate cur;
    Tile **tiles;
} Minefield;

typedef enum {
    alive,
    victory,
    dead
} Game_State;

#endif
