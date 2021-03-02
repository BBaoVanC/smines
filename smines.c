/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include "minefield.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "morecolor.h"
#include "colornames.h"

int main() {
    srand((unsigned) time(NULL)); /* create seed */

    /* ncurses setup */
    initscr(); /* start ncurses */
    if ((COLS < MCOLS) || (LINES < MROWS + 1)) {
        endwin();
        printf("Your terminal is too small for this minefield size.\n");
        printf("Your terminal is %i cols by %i rows, but %i cols and %i rows is required.\n", COLS, LINES, MCOLS, MROWS + 1);
        return 1;
    }
    keypad(stdscr, TRUE); /* more keys */
    noecho(); /* hide keys when pressed */
    curs_set(0); /* make the cursor invisible */

    start_color(); /* enable color */

    /* init_pair(id, fg, bg); */
    init_pair(TILE_ZERO,    COLOR_WHITE, COLOR_LIGHT_BLACK);
    init_pair(TILE_MINE,    COLOR_BLACK, COLOR_RED);

    init_pair(TILE_ONE,     COLOR_WHITE, COLOR_BLUE);
    init_pair(TILE_TWO,     COLOR_BLACK, COLOR_GREEN);
    init_pair(TILE_THREE,   COLOR_WHITE, COLOR_RED);
    init_pair(TILE_FOUR,    COLOR_BLACK, COLOR_CYAN);
    init_pair(TILE_FIVE,    COLOR_WHITE, 94);
    init_pair(TILE_SIX,     COLOR_BLACK, COLOR_MAGENTA);
    init_pair(TILE_SEVEN,   COLOR_WHITE, COLOR_BLACK);
    init_pair(TILE_EIGHT,   COLOR_WHITE, COLOR_LIGHT_BLACK);

    init_pair(TILE_HIDDEN,  COLOR_BLACK, COLOR_BLACK);
    init_pair(TILE_FLAG,    COLOR_BLACK, COLOR_YELLOW);
    init_pair(TILE_CURSOR,  COLOR_BLACK, COLOR_WHITE);

    init_pair(TILE_ERROR,   COLOR_WHITE, COLOR_RED);


    Minefield *minefield = NULL;
    int /* r ,*/ c;
    int start_r, start_c;

game:
    minefield = init_minefield(MROWS, MCOLS, MINES);

    start_r = minefield->rows/2;
    start_c = minefield->cols/2;

    populate_mines(minefield, start_r, start_c);

    generate_surrounding(minefield);
    reveal_tile(minefield, start_r, start_c); /* reveal the center tileA */

#ifdef TILE_COLOR_DEBUG
    for (int i = 0; i < 9; i++)
        minefield->tiles[i][0].visible = true;
    minefield->tiles[0][0].surrounding = 0;
    minefield->tiles[1][0].surrounding = 1;
    minefield->tiles[2][0].surrounding = 2;
    minefield->tiles[3][0].surrounding = 3;
    minefield->tiles[4][0].surrounding = 4;
    minefield->tiles[5][0].surrounding = 5;
    minefield->tiles[6][0].surrounding = 6;
    minefield->tiles[7][0].surrounding = 7;
    minefield->tiles[8][0].surrounding = 8;
#endif

    print_minefield(minefield, false);
    refresh();

    int cur_r, cur_c;
    Tile *cur_tile = NULL;
    int ch;
    while (true) {
        ch = getch(); /* wait for a character press */
        switch (ch) {
            case 'q': /* quit */
                goto quit;
                break;

            /* movement keys */
            case 'h':
            case KEY_LEFT:
                if (minefield->cur.col > 0)
                    minefield->cur.col--;
                break;
            case 'j':
            case KEY_DOWN:
                if (minefield->cur.row < minefield->rows - 1)
                    minefield->cur.row++;
                break;
            case 'k':
            case KEY_UP:
                if (minefield->cur.row > 0)
                    minefield->cur.row--;
                break;
            case 'l':
            case KEY_RIGHT:
                if (minefield->cur.col < minefield->cols - 1)
                    minefield->cur.col++;
                break;

            case ' ': /* reveal tile */
                cur_r = minefield->cur.row;
                cur_c = minefield->cur.col;
                if (!minefield->tiles[cur_r][cur_c].flagged) {
                    if (!reveal_tile(minefield, cur_r, cur_c)) {
                        reveal_mines(minefield);

                        clear();
                        print_minefield(minefield, true);
                        refresh();

                        mvprintw(minefield->rows, minefield->cols, "GAME OVER! Press `r` to play again.");
                        while (true) { /* wait for either 'q' to quit or 'r' to restart */
                            c = getch();
                            switch (c) {
                                case 'r':
                                    clear();
                                    goto game;
                                    break;
                                case 'q':
                                    goto quit;
                                    break;
                            }
                        }
                    }
                }
                break;

            case 'f': /* toggle flag */
                cur_r = minefield->cur.row;
                cur_c = minefield->cur.col;
                cur_tile = &minefield->tiles[cur_r][cur_c];
                if (!cur_tile->visible)
                    cur_tile->flagged = !cur_tile->flagged;
        }

        clear();
        print_minefield(minefield, false);
        refresh();
    }

quit:
    endwin();
    free(minefield);
}
