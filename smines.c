/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include "minefield.h"
#include "colors.h"
#include <ncurses.h>

Minefield *minefield;

void setup_minefield();

int main() {
    printf("Setting up the minefield");
    setup_minefield();

    printf("Initializing screen");
    initscr(); /* start ncurses */
    printf("Enabling extra keys with keypad()");
    keypad(stdscr, TRUE); /* more keys */
    printf("Hiding key output");
    noecho(); /* hide keys when pressed */

    printf("Setting up color");
    start_color(); /* start color */
    printf("Initializing color pairs");
    initcolorpairs(); /* set our color pairs */

    printf("Printing minefield");
    print_minefield(minefield);
    refresh();
}

void setup_minefield() {
    init_minefield(minefield, 10, 20);
}
