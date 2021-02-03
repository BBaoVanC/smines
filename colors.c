/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include <ncurses.h>

void initcolorpairs() {
    /* 10 is the 0 tile (0 can't be used for the pair number) */
    /* 9 is a mine */
    init_pair(10, COLOR_WHITE, COLOR_BLACK);
    init_pair(9, COLOR_BLACK, COLOR_RED);

    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_GREEN);
    init_pair(3, COLOR_WHITE, COLOR_RED);
    init_pair(4, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(5, COLOR_WHITE, COLOR_YELLOW);
    init_pair(6, COLOR_WHITE, COLOR_CYAN);
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
    init_pair(8, COLOR_BLACK, COLOR_WHITE);

    /* 11 is a hidden tile */
    /* 12 is a flag */
    init_pair(11, COLOR_WHITE, COLOR_BLACK);
    init_pair(12, COLOR_WHITE, COLOR_YELLOW);

    /* 100 is for errors */
    init_pair(100, COLOR_WHITE, COLOR_RED);
}

int getcolorforsurround(int surrounding) {
    if (surrounding == 0) {
        return COLOR_PAIR(10);
    } else if ((surrounding <= 8) && (surrounding >= 1)) {
        return COLOR_PAIR(surrounding);
    } else {
        return COLOR_PAIR(100);
    }
}
