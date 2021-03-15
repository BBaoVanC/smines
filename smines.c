/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "minefield.h"
#include "morecolor.h"
#include "colornames.h"
#include "window.h"
#include "types.h"
#include "draw.h"
#include "helper.h"

#include "global.h"

int SCOREBOARD_ROWS = 4;
Minefield *minefield = NULL;
WINDOW *fieldwin = NULL;
WINDOW *scorewin = NULL;
int origin_x, origin_y;
int game_number = 0; /* start at 0 because it's incremented before each game */
bool screen_too_small = FALSE;

int main() {
    srand((unsigned) time(NULL)); /* create seed */

    /* ncurses setup */
    initscr(); /* start ncurses */

    keypad(stdscr, TRUE); /* more keys */
    noecho(); /* hide keys when pressed */
    curs_set(0); /* make the cursor invisible */
    refresh(); /* if I don't do this, the window doesn't appear until a key press */

    start_color(); /* enable color */
    use_default_colors();

    /* init_pair(id, fg, bg); */
    init_pair(TILE_ZERO,    COLOR_WHITE,    COLOR_BLACK);
    init_pair(TILE_MINE,    COLOR_RED,      COLOR_BLACK);

    init_pair(TILE_ONE,     COLOR_WHITE,    COLOR_BLUE);
    init_pair(TILE_TWO,     COLOR_BLACK,    COLOR_GREEN);
    init_pair(TILE_THREE,   COLOR_WHITE,    COLOR_RED);
    init_pair(TILE_FOUR,    COLOR_BLACK,    COLOR_CYAN);
    init_pair(TILE_FIVE,    COLOR_WHITE,    94);
    init_pair(TILE_SIX,     COLOR_BLACK,    COLOR_MAGENTA);
    init_pair(TILE_SEVEN,   COLOR_WHITE,    COLOR_BLACK);
    init_pair(TILE_EIGHT,   COLOR_WHITE,    COLOR_LIGHT_BLACK);

    init_pair(TILE_HIDDEN,  COLOR_LIGHT_BLACK,    -1);
    init_pair(TILE_FLAG,    COLOR_YELLOW,   COLOR_BLACK);
    init_pair(TILE_CURSOR,  COLOR_BLACK,    COLOR_WHITE);

    init_pair(TILE_ERROR,   COLOR_WHITE,    COLOR_RED);

    init_pair(MSG_DEATH,    COLOR_RED,      -1);
    init_pair(MSG_WIN,      COLOR_GREEN,    -1);


    set_origin(); /* find what coordinate to start at */
    /* add 2 to each dimension on every window to fit the
     * borders (since they are inside borders) */
    fieldwin = newwin(MROWS + 2, MCOLS*2 + 2, origin_y + SCOREBOARD_ROWS, origin_x);
    wrefresh(fieldwin);

    scorewin = newwin(SCOREBOARD_ROWS, MCOLS*2, origin_y, origin_x);
    wrefresh(scorewin);

game:
    if (++game_number != 1) /* we don't need to free the first game because we haven't started yet
                             * also might as well increment game_number while we're here */
        free(minefield);

    minefield = init_minefield(MROWS, MCOLS, MINES);

    int start_r = minefield->rows/2;
    int start_c = minefield->cols/2;

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

    draw_screen();

    int cur_r, cur_c;
    int r, c;
    Tile *cur_tile = NULL;
    int ch;
    while (true) {
        ch = getch(); /* wait for a character press */
        if (ch == KEY_RESIZE) {
            nodelay(stdscr, 1); /* delay can cause the field to go invisible when resizing quickly */
            resize_screen();
            draw_screen();
            continue;
        }

        nodelay(stdscr, 0); /* we want to wait until a key is pressed */
        switch (ch) {
            case 'L':
                resize_screen();
                draw_screen();
                break;

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

            case '0':
            case '^':
                minefield->cur.col = 0;
                break;
            case '$':
                minefield->cur.col = minefield->cols - 1;
                break;
            case 'g':
                minefield->cur.row = 0;
                break;
            case 'G':
                minefield->cur.row = minefield->rows - 1;
                break;

            case ' ': /* reveal tile */
                cur_r = minefield->cur.row;
                cur_c = minefield->cur.col;
                cur_tile = &minefield->tiles[cur_r][cur_c];
                if (cur_tile->visible) {
                    if (getflagsurround(minefield, cur_r, cur_c) == cur_tile->surrounding) {
                        for (r = cur_r - 1; r < cur_r + 2; r++) {
                            for (c = cur_c - 1; c < cur_c + 2; c++) {
                                if (!minefield->tiles[r][c].flagged) { /* in rare cases this causes an invalid read error
                                                                        * could be fixed by moving this under the
                                                                        * range check (the if statement below) */
                                    if ((r >= 0 && c >= 0) && (r < minefield->rows && c < minefield->cols)) {
                                        if (!reveal_tile(minefield, r, c)) {
                                            if (death(minefield, fieldwin, scorewin, game_number))
                                                goto game;
                                            else
                                                goto quit;
                                        }
                                    }
                                }
                            }
                        }
                    }
                } else if (!cur_tile->flagged) {
                    if (!reveal_tile(minefield, cur_r, cur_c)) {
                        if (death(minefield, fieldwin, scorewin, game_number))
                            goto game;
                        else
                            goto quit;
                    }
                }
                if (check_victory(minefield)) {
                    if (victory(minefield, fieldwin, scorewin, game_number))
                        goto game;
                    else
                        goto quit;
                }
                break;

            case 'f': /* toggle flag */
                cur_r = minefield->cur.row;
                cur_c = minefield->cur.col;
                cur_tile = &minefield->tiles[cur_r][cur_c];
                if (!cur_tile->visible) {
                    cur_tile->flagged = !cur_tile->flagged;
                    if (cur_tile->flagged)
                        minefield->placed_flags++;
                    else
                        minefield->placed_flags--;
                }

                if (check_victory(minefield)) {
                    if (victory(minefield, fieldwin, scorewin, game_number))
                        goto game;
                    else
                        goto quit;
                }
                break;
        }

        draw_screen();
    }

quit:
    endwin();
}
