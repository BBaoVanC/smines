/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include "minefield.h"
#include "colors.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

Minefield *minefield;

void setup_minefield();

int main() {
    Minefield *minefield = malloc(sizeof(Minefield));
    printf("Setting up the minefield\n");
    setup_minefield();

    printf("Initializing screen\n");
    initscr(); /* start ncurses */
    printf("Enabling extra keys with keypad()\n");
    keypad(stdscr, TRUE); /* more keys */
    printf("Hiding key output\n");
    noecho(); /* hide keys when pressed */

    printf("Setting up color\n");
    start_color(); /* start color */
    printf("Initializing color pairs\n");
    initcolorpairs(); /* set our color pairs */

    printf("Printing minefield\n");
    print_minefield(minefield);
    refresh();
}

void setup_minefield() {
    init_minefield(minefield, 10, 20);
}
