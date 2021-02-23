/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include <stdbool.h>
#include "config.h"

typedef struct {
    int col;
    int row;
} Cursor;

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
    Cursor cur;
    Tile tiles[MROWS][MCOLS];
} Minefield;

Minefield *init_minefield(int, int, int);
void populate_mines(Minefield *);
void generate_surrounding(Minefield *);
int getcolorforsurround(int);
void print_tile(Tile *);
void print_cursor_tile(Tile *);
void print_minefield(Minefield *);
bool reveal_tile(Minefield *, int, int);
void reveal_mines(Minefield *);
int getsurround(Minefield *, int, int);
