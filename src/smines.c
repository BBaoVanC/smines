/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include "colornames.h"
#include "draw.h"
#include "help.h"
#include "minefield.h"
#include "morecolor.h"
#include "types.h"
#include "window.h"

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <getopt.h>

// TODO: should be set at runtime instead of compile time
#if ALLOW_UNDO
    #include "undo.h"
#endif

// TODO: no global state
int SCOREBOARD_ROWS = 4;
int origin_x, origin_y;
int game_number = 0; /* start at 0 because it's incremented before each game */
bool screen_too_small = FALSE;
Game_State game_state;

bool help_visible = false; /* if true, draw help page **instead of** everything else */

#if ALLOW_UNDO
struct Minefield undo_minefield;   /* the minefield before the last move */
Game_State undo_game_state; /* the game state before the last move */
#endif

static void draw_screen(struct Minefield *minefield) {
    int min_rows = SCOREBOARD_ROWS + minefield->rows + 2; /* add 2 to fit the minefield borders */
    int min_cols = minefield->cols * 2 + 2;               /* multiply cols by 2 because each tile is 2 cols wide */

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

static void set_origin(struct Minefield *minefield) {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    int height = minefield->rows + SCOREBOARD_ROWS * 2; /* center based on the minefield, ignoring the scoreboard */
    int width = minefield->cols * 2 + 2;                /* add 2 because we're adding 1 to each side to fit borders */

    origin_x = (cols - width) / 2;
    origin_y = (rows - height) / 2;

    /* make sure we don't try and start drawing past the top and left sides */
    if (origin_x < 0)
        origin_x = 0;
    if (origin_y < 0)
        origin_y = 0;
}

static void resize_screen(struct Minefield *minefield) {
    destroy_win(fieldwin);
    destroy_win(scorewin);

    endwin(); /* this makes ncurses recalculate things, such as the global variables LINES and COLS */
    set_origin();

    fieldwin = newwin(minefield->rows + 2, minefield->cols * 2 + 2, origin_y + SCOREBOARD_ROWS, origin_x);
    wborder(fieldwin, 0, 0, 0, 0, 0, 0, 0, 0);
    wrefresh(fieldwin);

    scorewin = newwin(SCOREBOARD_ROWS, minefield->cols * 2, origin_y, origin_x);
    wrefresh(scorewin);
}

static void reveal_check_state(struct Minefield *minefield, size_t row, size_t col) {
    if (!reveal_tile(minefield, row, col)) {
        game_state = dead;
        reveal_mines(minefield);
    } else if (check_victory(minefield)) {
        game_state = victory;
        int r, c;
        for (r = 0; r < minefield->rows; r++) {
            for (c = 0; c < minefield->cols; c++) {
                minefield_get_tile(minefield, r, c)->visible = true;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    // should these all be static
    const char cmd_usage[] =
        "Usage: smines [options]\n"
    ;
    const char cmd_help[] =
        "Options:\n"
        "  -h, --help\n"
    ;

    int help_flag = 0;
    int undo_flag = 0;
    const struct option long_options[] = {
        { "help",       no_argument,        &help_flag, 1   },
        { "rows",       required_argument,  0,          'r' },
        { "cols",       required_argument,  0,          'c' },
        { "mines",      required_argument,  0,          'm' },
        { "difficulty", required_argument,  0,          'd' },
        { "allow-undo", no_argument,        &undo_flag, 1   },
        { 0, 0, 0, 0 }
    };
    // TODO: make these unsigned and also use stdint
    // TODO: check if not provided and error if not
    int rows, cols, mines;

    bool exit_for_invalid_args = false;
    int opt_idx = 0;
    int c;
    while ((c = getopt_long(argc, argv, "hr:c:d:u", long_options, &opt_idx)) != -1) {
        switch (c) {
            case 0:
                // do nothing else if flag was set
                if (long_options[opt_idx].flag != 0) {
                    break;
                }
                abort();
            case '?':
                exit_for_invalid_args = true;
                break;
            case 'h':
                help_flag = 1;
                break;
            case 'r':
                rows = atoi(optarg);
                if (rows < 0) {
                    printf("'rows' cannot be negative!\n");
                    return 1;
                }
                break;
            case 'c':
                cols = atoi(optarg);
                if (cols < 0) {
                    printf("'cols' cannot be negative!\n");
                    return 1;
                }
                break;
            case 'm':
                mines = atoi(optarg);
                if (mines < 0) {
                    printf("'mines' cannot be negative!\n");
                    return 1;
                }
                break;
            case 'd':
                printf("unimplemented difficulty setting\n");
                abort();
            case 'u':
                undo_flag = 1;
                break;
            default:
                abort();
        }
    }
    if (exit_for_invalid_args) {
        printf(cmd_usage);
        printf("Use the '--help' option to display help page\n");
        return 1;
    }
    if (help_flag) {
        printf(cmd_usage);
        printf(cmd_help);
        return 0;
    }
    // TODO: print out non option args

    srand((unsigned)time(NULL)); /* seed the random number generator */

    /* ncurses setup */
    initscr(); /* start ncurses */

    if (!has_colors()) {
        endwin();
        printf("smines requires color support in your terminal to work properly.\n");
        exit(1);
    }

    keypad(stdscr, TRUE); /* for the arrow keys */
    noecho();             /* don't show the letters of pressed keys */
    curs_set(0);          /* make the cursor invisible */
    refresh();            /* if I don't do this, the window doesn't appear until a key press */

    start_color();        /* enable color */
    use_default_colors(); /* allows us to use colors like the terminal background */

    /* init_pair(id, fg, bg); */
    init_pair(TILE_ZERO, COLOR_WHITE, COLOR_BLACK);
    init_pair(TILE_MINE, COLOR_RED, COLOR_BLACK);
    init_pair(TILE_MINE_SAFE, COLOR_GREEN, COLOR_BLACK);

    init_pair(TILE_ONE, COLOR_WHITE, COLOR_BLUE);
    init_pair(TILE_TWO, COLOR_BLACK, COLOR_GREEN);
    init_pair(TILE_THREE, COLOR_WHITE, COLOR_RED);
    init_pair(TILE_FOUR, COLOR_BLACK, COLOR_CYAN);
    init_pair(TILE_FIVE, COLOR_WHITE, 94);
    init_pair(TILE_SIX, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(TILE_SEVEN, COLOR_WHITE, COLOR_BLACK);
    init_pair(TILE_EIGHT, COLOR_WHITE, COLOR_LIGHT_BLACK);

    init_pair(TILE_HIDDEN, COLOR_LIGHT_BLACK, -1);
    init_pair(TILE_FLAG, COLOR_YELLOW, COLOR_BLACK);
    init_pair(TILE_FLAG_WRONG, COLOR_BLUE, COLOR_BLACK);
    init_pair(TILE_CURSOR, COLOR_BLACK, COLOR_WHITE);

    init_pair(TILE_ERROR, COLOR_WHITE, COLOR_RED);

    init_pair(MSG_DEATH, COLOR_RED, -1);
    init_pair(MSG_WIN, COLOR_GREEN, -1);

    set_origin(); /* find what coordinate to start at */

    /* add 2 to each dimension on every window to fit the
     * borders (since they are inside borders) */
    fieldwin = newwin(rows + 2, cols * 2 + 2, origin_y + SCOREBOARD_ROWS, origin_x);
    wrefresh(fieldwin);

    scorewin = newwin(SCOREBOARD_ROWS, cols * 2, origin_y, origin_x);
    wrefresh(scorewin);

game:
    game_state = alive;
    game_number++; // starts at 0 but immediately increment to 1 for first game

    // TODO: should accept a pointer to Minefield to init, then stack allocate instead
    struct Minefield minefield;
    minefield_init(&minefield, rows, cols, mines);
    bool first_reveal = true;

#if ALLOW_UNDO
    copy_undo();
#endif

    draw_screen();

    struct Tile *cur_tile = NULL; /* pointer to the tile the cursor is on */
    int ch; /* key that was pressed */
    while (true) {
        cur_tile = minefield_get_tile(&minefield, minefield.cur.row, minefield.cur.col);
        ch = getch(); /* blocks until a key is pressed */
        if (ch == KEY_RESIZE) {
            nodelay(stdscr, 1); /* delay can cause the field to go invisible when resizing quickly */
            if (help_visible) {
                clear();
                draw_help(stdscr);
            } else {
                resize_screen();
                draw_screen();
            }
            continue; /* immediately start reading another key press */
        }

        nodelay(stdscr, 0); /* we want to wait until a key is pressed */
        if (help_visible) {
            switch (ch) {
                case 'H': /* close help */
                case '?':
                case 'q':
                    clear();
                    refresh();
                    help_visible = !help_visible;
                    draw_screen();
                    break;
            }
            continue; /* then I don't need to use else below and
                         add an entire level of indentation to
                         everything */
        }
        switch (ch) {
            case 'L': /* redraw screen, just in case */
                resize_screen();
                break;

            case 'q': /* quit */
                goto quit;
                break;

            case 'r': /* restart */
                goto game;
                break;

            case 'H': /* toggle help, only checked here if not visible already */
            case '?':
                clear();
                refresh();
                help_visible = !help_visible;
                break;

            /* movement keys */
            case 'h':
            case KEY_LEFT:
                if (minefield.cur.col > 0)
                    minefield.cur.col--;
                break;
            case 'j':
            case KEY_DOWN:
                if (minefield.cur.row < minefield.rows - 1)
                    minefield.cur.row++;
                break;
            case 'k':
            case KEY_UP:
                if (minefield.cur.row > 0)
                    minefield.cur.row--;
                break;
            case 'l':
            case KEY_RIGHT:
                if (minefield.cur.col < minefield.cols - 1)
                    minefield.cur.col++;
                break;

            case '0':
            case '^':
                minefield.cur.col = 0;
                break;
            case '$':
                minefield.cur.col = minefield.cols - 1;
                break;
            case 'g':
                minefield.cur.row = 0;
                break;
            case 'G':
                minefield.cur.row = minefield.rows - 1;
                break;

#if ALLOW_UNDO
            case 'u': /* undo */
                undo();
                draw_screen();
                break;
#endif

            case ' ': /* reveal tile */
                if (first_reveal) {
                    // TODO: add these back lmao
                    minefield_populate(&minefield);
                    minefield_reveal_tile(&minefield, minefield.cur.row, minefield.cur.col);
                    first_reveal = false;
#if ALLOW_UNDO
                    copy_undo();
#endif
                    break;
                }
                if (game_state != alive)
                    break;
                if (cur_tile->visible) {
                    if (minefield_count_surrounding_flags(&minefield, minefield.cur.row, minefield.cur.col) == cur_tile->surrounding) {
#if ALLOW_UNDO
                        copy_undo();
#endif
                        /* If you have x flags surrounding an already revealed tile, and
                         * that tile has x surrounding mines, then the other surrounding
                         * tiles cannot be mines (assuming your flags are correct).
                         * If any of the flags are incorrect, then you die.
                         * Other versions seem to do it this way too, and it's easy to
                         * program it this way. */
                        for (int r = minefield.cur.row - 1; r < minefield.cur.row + 2; r++) {
                            for (int c = minefield.cur.col - 1; c < minefield.cur.col + 2; c++) {
                                if ((r >= 0 && c >= 0) && (r < minefield.rows && c < minefield.cols)) {
                                    if (!minefield_get_tile(&minefield, r, c)->flagged) {
                                        reveal_check_state(r, c);
                                    }
                                }
                            }
                        }
                    }
                } else if (!cur_tile->flagged) {
#if ALLOW_UNDO
                    copy_undo();
#endif
                    reveal_check_state(minefield.cur.row, minefield.cur.col);
                }
                break;

            case 'f': /* toggle flag */
                if (game_state != alive)
                    break;
                if (!cur_tile->visible) {
                    cur_tile->flagged = !cur_tile->flagged;
                    if (cur_tile->flagged)
                        minefield.placed_flags++;
                    else
                        minefield.placed_flags--;
                }
                break;
        }

        draw_screen();
    }

quit:
    endwin();
}
