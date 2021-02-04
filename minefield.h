/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

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
    int cols;
    int rows;
    int mines;
    Tile *tiles;
} Minefield;

Tile *get_tile_ptr(Minefield *, int, int);
void init_minefield(Minefield *, int, int);
int getcolorforsurround(int);
void print_tile(Tile *);
void print_minefield(Minefield *);
int getsurround(Minefield *, int, int);
