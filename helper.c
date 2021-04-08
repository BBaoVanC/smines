/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include "draw.h"
#include "help.h"
#include "window.h"

#include "global.h"

void draw_screen() {
    int min_rows = SCOREBOARD_ROWS + MROWS + 2; /* add 2 to fit the minefield borders */
    int min_cols = MCOLS * 2 + 2;               /* multiply cols by 2 because each tile is 2 cols wide */

    if ((LINES < min_rows) || (COLS < min_cols)) {
        screen_too_small = TRUE;
        clear(); /* we don't want any leftover pieces of the minefield */
        mvprintw(0, 0, "Please make your terminal at least %i cols by %i rows\n", min_cols, min_rows);
        printw("Current size: %i cols by %i rows", COLS, LINES);
        refresh();
    } else if (help_visible) {
        draw_help(stdscr);
        refresh();
    } else {
        if (screen_too_small) {
            clear();
            screen_too_small = FALSE;
        }

        draw_minefield(fieldwin, minefield, game_state);
        wborder(fieldwin, 0, 0, 0, 0, 0, 0, 0, 0);
        wrefresh(fieldwin);

        draw_scoreboard(scorewin, minefield, game_number, game_state);
        wrefresh(scorewin);
    }
}

void set_origin() {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    int height = MROWS + SCOREBOARD_ROWS * 2; /* center based on the minefield, ignoring the scoreboard */
    int width = MCOLS * 2 + 2;                /* add 2 because we're adding 1 to each side to fit borders */

    origin_x = (cols - width) / 2;
    origin_y = (rows - height) / 2;

    /* make sure we don't try and start drawing past the top and left sides */
    if (origin_x < 0)
        origin_x = 0;
    if (origin_y < 0)
        origin_y = 0;
}

void resize_screen() {
    destroy_win(fieldwin);
    destroy_win(scorewin);

    endwin(); /* this makes ncurses recalculate things, such as the global variables LINES and COLS */
    set_origin();

    fieldwin = newwin(MROWS + 2, MCOLS * 2 + 2, origin_y + SCOREBOARD_ROWS, origin_x);
    wborder(fieldwin, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(fieldwin);

    scorewin = newwin(SCOREBOARD_ROWS, MCOLS * 2, origin_y, origin_x);
    wrefresh(scorewin);
}

void reveal_check_state(int row, int col) {
    if (!reveal_tile(minefield, row, col)) {
        game_state = dead;
        reveal_mines(minefield);
    } else if (check_victory(minefield)) {
        game_state = victory;
        int r, c;
        for (r = 0; r < minefield->rows; r++) {
            for (c = 0; c < minefield->cols; c++) {
                minefield->tiles[r][c].visible = true;
            }
        }
    }
}
