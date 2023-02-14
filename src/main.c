#include "display.h"
#include "game.h"
#include "minefield.h"

#include <getopt.h>
#include <ncurses.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h> // strcasecmp
#include <time.h>

static void reveal_check_state(struct Game *game, size_t row, size_t col) {
    if (!minefield_reveal_tile(&game->minefield, row, col)) {
        game->state = DEAD;
        // reveal all the mines
        for (size_t r = 0; r < game->minefield.rows; r++) {
            for (size_t c = 0; c < game->minefield.cols; c++) {
                if (minefield_get_tile(&game->minefield, r, c)->mine) {
                    minefield_get_tile(&game->minefield, r, c)->visible = true;
                }
            }
        }
    } else if (minefield_check_victory(&game->minefield)) {
        game->state = VICTORY;
        int r, c;
        for (r = 0; r < game->minefield.rows; r++) {
            for (c = 0; c < game->minefield.cols; c++) {
                minefield_get_tile(&game->minefield, r, c)->visible = true;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    // https://stackoverflow.com/questions/38462701/why-declare-a-static-variable-in-main
    static const char cmd_usage[] =
        "Usage: smines [options]\n"
    ;
    static const char cmd_help[] =
        "Options:\n"
        "  -h, --help\n"
        "  -r, --rows=ROWS                  Set the amount of rows in the minefield\n"
        "  -c, --cols=COLS                  Set the amount of columns in the minefield\n"
        "  -m, --mines=MINES                Set the amount of mines in the minefield\n"
        "  -d, --difficulty=DIFFICULTY      Set the rows, columns, and mines based on difficulty level\n"
        "  -u, --allow-undo                 Allow undoing the last move\n"
    ;

    static int help_flag = 0;
    static int undo_flag = 0;
    static const struct option long_options[] = {
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
    int rows = -1;
    int cols = -1;
    int mines = -1;

    bool exit_for_invalid_args = false;
    int opt_idx = 0;
    char *strtol_endptr;
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
                errno = 0;
                rows = strtol(optarg, &strtol_endptr, 10);
                // TODO: check for more errors (such as "52jkfndnkj" being read as 52)
                if (optarg == strtol_endptr || errno != 0) {
                    printf("error parsing 'rows' as number\n");
                    exit_for_invalid_args = true;
                }
                if (rows < 0) {
                    printf("'rows' cannot be negative!\n");
                    exit_for_invalid_args = true;
                }
                break;
            case 'c':
                errno = 0;
                cols = strtol(optarg, &strtol_endptr, 10);
                if (optarg == strtol_endptr || errno != 0) {
                    printf("error parsing 'cols' as number\n");
                    exit_for_invalid_args = true;
                }
                if (cols < 0) {
                    printf("'cols' cannot be negative!\n");
                    exit_for_invalid_args = true;
                }
                break;
            case 'm':
                errno = 0;
                mines = strtol(optarg, &strtol_endptr, 10);
                if (optarg == strtol_endptr || errno != 0) {
                    printf("error parsing 'mines' as number\n");
                    exit_for_invalid_args = true;
                }
                if (mines < 0) {
                    printf("'mines' cannot be negative!\n");
                    exit_for_invalid_args = true;
                }
                break;
            case 'd':
                if (strcasecmp(optarg, "easy") == 0) {
                    rows = 9;
                    cols = 9;
                    mines = 10;
                } else if (strcasecmp(optarg, "super-easy") == 0 || strcasecmp(optarg, "super_easy") == 0) {
                    rows = 10;
                    cols = 20;
                    mines = 10;
                } else if (strcasecmp(optarg, "intermediate") == 0 || strcasecmp(optarg, "medium") == 0) {
                    rows = 16;
                    cols = 16;
                    mines = 40;
                } else if (strcasecmp(optarg, "hard") == 0) {
                    rows = 16;
                    cols = 30;
                    mines = 99;
                } else {
                    printf("invalid difficulty: %s\n", optarg);
                    exit_for_invalid_args = true;
                }
                break;
            case 'u':
                undo_flag = 1;
                break;
            default:
                abort();
        }
    }
    if (rows == -1) {
        printf("'rows' was not set, use --rows or --difficulty\n");
        exit_for_invalid_args = true;
    }
    if (cols == -1) {
        printf("'cols' was not set, use --cols or --difficulty\n");
        exit_for_invalid_args = true;
    }
    if (mines == -1) {
        printf("'mines' was not set, use --mines or --difficulty\n");
        exit_for_invalid_args = true;
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

    if (optind < argc) {
        printf("non-option arguments were found: ");
        for (; optind < argc; optind++) {
            printf("%s ", argv[optind]);
        }
        putchar('\n');
    }

    if (mines > (rows * cols) - 9) { // subtract 9 because mines can't be around the start
        printf("minefield is not large enough to fit the requested amount of mines\n");
        return 1;
    }
    if (rows < 5) {
        printf("'rows' must be at least 5\n");
        return 1;
    }
    if (cols < 5) {
        printf("'cols' must be at least 5\n");
        return 1;
    }

    srand((unsigned)time(NULL)); // seed the random number generator

    struct Display display;
    display_init(&display, rows, cols);

    struct Game game = {0};

game:
    game.state = ALIVE;
    game.game_number++;

    minefield_init(&game.minefield, rows, cols, mines);
    bool first_reveal = true;

    struct Tile *cur_tile = NULL; // pointer to the tile the cursor is on
    int ch; // key that was pressed
    while (true) {
        display_draw(&display, &game);
        refresh();
        wrefresh(display.scoreboard);
        wrefresh(display.minefield);
        cur_tile = minefield_get_tile(&game.minefield, game.minefield.cur.row, game.minefield.cur.col);
        ch = getch(); // blocks until a key is pressed
        if (ch == KEY_RESIZE) {
            display_resize(&display, game.minefield.rows, game.minefield.cols);
            continue;
        }

        nodelay(stdscr, 0); // we want to wait until a key is pressed
        if (display.state == HELP) {
            switch (ch) {
                case 'H': // close help
                case '?':
                case 'q':
                    erase();
                    display.state = GAME;
                    display_draw(&display, &game);
                    break;
            }
            continue;
        }
        switch (ch) {
            case 'L': // redraw screen
                display_resize(&display, game.minefield.rows, game.minefield.cols);
                display_draw(&display, &game);
                refresh();
                wrefresh(display.minefield);
                wrefresh(display.scoreboard);
                break;

            case 'q': // quit
                goto quit;
                break;

            case 'r': // restart
                goto game;
                break;

            case 'H': // toggle help, only checked here if not visible already
            case '?':
                erase();
                display.state = HELP;
                break;

            // movement keys
            case 'h':
            case KEY_LEFT:
                if (game.minefield.cur.col > 0)
                    game.minefield.cur.col--;
                break;
            case 'j':
            case KEY_DOWN:
                if (game.minefield.cur.row < game.minefield.rows - 1)
                    game.minefield.cur.row++;
                break;
            case 'k':
            case KEY_UP:
                if (game.minefield.cur.row > 0)
                    game.minefield.cur.row--;
                break;
            case 'l':
            case KEY_RIGHT:
                if (game.minefield.cur.col < game.minefield.cols - 1)
                    game.minefield.cur.col++;
                break;

            case '0':
            case '^':
                game.minefield.cur.col = 0;
                break;
            case '$':
                game.minefield.cur.col = game.minefield.cols - 1;
                break;
            case 'g':
                game.minefield.cur.row = 0;
                break;
            case 'G':
                game.minefield.cur.row = game.minefield.rows - 1;
                break;

            case 'u': // undo
                if (undo_flag) {
                    game_undo(&game);
                    display_draw(&display, &game);
                }
                break;

            case ' ': // reveal tile
                if (first_reveal) {
                    // TODO: add these back lmao
                    minefield_populate(&game.minefield);
                    minefield_reveal_tile(&game.minefield, game.minefield.cur.row, game.minefield.cur.col);
                    first_reveal = false;
                    game_undo_store(&game);
                    break;
                }
                if (game.state != ALIVE)
                    break;
                if (cur_tile->visible) {
                    if (minefield_count_surrounding_flags(&game.minefield, game.minefield.cur.row, game.minefield.cur.col) == cur_tile->surrounding) {
                        game_undo_store(&game);
                        /* If you have x flags surrounding an already revealed tile, and
                         * that tile has x surrounding mines, then the other surrounding
                         * tiles cannot be mines (assuming your flags are correct).
                         * If any of the flags are incorrect, then you die. */
                        for (int r = game.minefield.cur.row - 1; r < game.minefield.cur.row + 2; r++) {
                            for (int c = game.minefield.cur.col - 1; c < game.minefield.cur.col + 2; c++) {
                                if ((r >= 0 && c >= 0) && (r < game.minefield.rows && c < game.minefield.cols)) {
                                    if (!minefield_get_tile(&game.minefield, r, c)->flagged) {
                                        reveal_check_state(&game, r, c);
                                    }
                                }
                            }
                        }
                    }
                } else if (!cur_tile->flagged) {
                    game_undo_store(&game);
                    reveal_check_state(&game, game.minefield.cur.row, game.minefield.cur.col);
                }
                break;

            case 'f': // toggle flag
                if (game.state != ALIVE)
                    break;
                if (!cur_tile->visible) {
                    cur_tile->flagged = !cur_tile->flagged;
                    if (cur_tile->flagged)
                        game.minefield.placed_flags++;
                    else
                        game.minefield.placed_flags--;
                }
                break;
        }
    }

quit:
    minefield_cleanup(&game.minefield);
    display_destroy(&display);
}
