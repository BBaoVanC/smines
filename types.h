/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#ifndef __SMINES_TYPES_H
#define __SMINES_TYPES_H

#include "config.h"

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
    Tile tiles[MROWS][MCOLS];
} Minefield;

enum States {
    alive,
    victory,
    dead
};

#endif
