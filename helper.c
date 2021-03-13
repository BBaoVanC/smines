/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include "draw.h"
#include "window.h"

void draw_screen() {
    extern int SCOREBOARD_ROWS;
    extern WINDOW *fieldwin;
    extern WINDOW *scorewin;
    extern Minefield *minefield;
    extern bool screen_too_small;
    extern int game_number;

    int min_rows = SCOREBOARD_ROWS + MROWS + 2;
    int min_cols = MCOLS*2 + 2;
    if ((LINES >= min_rows) && (COLS >= min_cols)) {
        if (screen_too_small) {
            clear();
            screen_too_small = FALSE;
        }
        draw_minefield(fieldwin, minefield, false);
        wborder(fieldwin, 0, 0, 0, 0, 0, 0, 0, 0);
        wrefresh(fieldwin);

        draw_scoreboard(scorewin, minefield, game_number);
        wrefresh(scorewin);
    } else {
        screen_too_small = TRUE;
        clear();
        mvprintw(0, 0, "Please make your terminal at least %i cols by %i rows\n", min_cols, min_rows);
        printw("Current size: %i cols by %i rows", COLS, LINES);
        refresh();
    }
}

void set_origin() {
    extern int SCOREBOARD_ROWS;
    extern int origin_x, origin_y;

    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    int height = MROWS + SCOREBOARD_ROWS * 2; /* center based on the minefield, ignoring the scoreboard */
    int width = MCOLS*2 + 2; /* add 2 because we're adding 1 to each side to fit borders */

    origin_x = (cols - width) / 2;
    origin_y = (rows - height) / 2;

    if (origin_x < 0)
        origin_x = 0;
    if (origin_y < 0)
        origin_y = 0;
}

void resize_screen() {
    extern WINDOW *fieldwin;
    extern WINDOW *scorewin;
    extern int SCOREBOARD_ROWS;
    extern int origin_x, origin_y;

    destroy_win(fieldwin);
    destroy_win(scorewin);

    endwin();
    set_origin();

    fieldwin = newwin(MROWS + 2, MCOLS*2 + 2, origin_y + SCOREBOARD_ROWS, origin_x);
    wborder(fieldwin, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(fieldwin);

    scorewin = newwin(SCOREBOARD_ROWS, MCOLS*2, origin_y, origin_x);
    wrefresh(scorewin);
}
