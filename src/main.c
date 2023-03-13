#include "display.h"
#include "game.h"
#include "minefield.h"

#include <getopt.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h> // strcasecmp
#include <time.h>

int main(int argc, char *argv[]) {
    // https://stackoverflow.com/questions/38462701/why-declare-a-static-variable-in-main
    static const char cmd_usage[] =
        "Usage: smines [options]\n"
    ;
    static const char cmd_help[] =
        "Options:\n"
        "  -h, --help\n"
        "  -c, --cols=WIDTH                 Set the width of the minefield\n"
        "  -r, --rows=HEIGHT                Set the height of the minefield\n"
        "  -m, --mines=MINES                Set the amount of mines in the minefield\n"
        "  -d, --difficulty=DIFFICULTY      Set the rows, columns, and mines based on difficulty level\n"
        "  -u, --allow-undo                 Allow undoing the last move\n"
        "Difficulties:\n"
        "  super-easy, super_easy   20x10, 10 mines\n"
        "  easy                     9x9,   10 mines\n"
        "  intermediate, medium     16x16, 40 mines\n"
        "  hard                     30x16, 99 mines\n"
    ;

    static int help_flag = 0;
    static int undo_flag = 0;
    static const struct option long_options[] = {
        { "help",       no_argument,        &help_flag, 1   },
        { "cols",       required_argument,  0,          'r' },
        { "rows",       required_argument,  0,          'c' },
        { "mines",      required_argument,  0,          'm' },
        { "difficulty", required_argument,  0,          'd' },
        { "allow-undo", no_argument,        &undo_flag, 1   },
        { 0, 0, 0, 0 }
    };
    // TODO: make these unsigned and also use stdint
    // TODO: check if not provided and error if not
    int width = -1;
    int height = -1;
    int mines = -1;

    bool exit_for_invalid_args = false;
    int opt_idx = 0;
    char *strtol_endptr;
    int c;
    while ((c = getopt_long(argc, argv, "hc:r:m:d:u", long_options, &opt_idx)) != -1) {
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
            case 'c':
                errno = 0;
                width = strtol(optarg, &strtol_endptr, 10);
                if (optarg == strtol_endptr || errno != 0) {
                    printf("error parsing 'width' as number\n");
                    exit_for_invalid_args = true;
                }
                if (width < 0) {
                    printf("'width' cannot be negative!\n");
                    exit_for_invalid_args = true;
                }
                break;
            case 'r':
                errno = 0;
                height = strtol(optarg, &strtol_endptr, 10);
                // TODO: check for more errors (such as "52jkfndnkj" being read as 52)
                if (optarg == strtol_endptr || errno != 0) {
                    printf("error parsing 'height' as number\n");
                    exit_for_invalid_args = true;
                }
                if (height < 0) {
                    printf("'height' cannot be negative!\n");
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
                    width = 9;
                    height = 9;
                    mines = 10;
                } else if (strcasecmp(optarg, "super-easy") == 0 || strcasecmp(optarg, "super_easy") == 0) {
                    width = 20;
                    height = 10;
                    mines = 10;
                } else if (strcasecmp(optarg, "intermediate") == 0 || strcasecmp(optarg, "medium") == 0) {
                    width = 16;
                    height = 16;
                    mines = 40;
                } else if (strcasecmp(optarg, "hard") == 0) {
                    width = 30;
                    height = 16;
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
    if (help_flag) {
        printf(cmd_usage);
        printf(cmd_help);
        return 0;
    }

    if (width == -1) {
        printf("'width' was not set, use --width or --difficulty\n");
        exit_for_invalid_args = true;
    }
    if (height == -1) {
        printf("'height' was not set, use --height or --difficulty\n");
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

    if (optind < argc) {
        printf("non-option arguments were found: ");
        for (; optind < argc; optind++) {
            printf("%s ", argv[optind]);
        }
        putchar('\n');
    }

    if (mines > (width * height) - 9) { // subtract 9 because mines can't be around the start
        printf("minefield is not large enough to fit the requested amount of mines\n");
        return 1;
    }
    if (width < 5) {
        printf("'width' must be at least 5\n");
        return 1;
    }
    if (height < 5) {
        printf("'height' must be at least 5\n");
        return 1;
    }

    srand((unsigned)time(NULL)); // seed the random number generator

    struct Display display;
    display_init(&display);
    nodelay(stdscr, 0); // block while waiting for key press

    struct Game game = {0};
    bool restart_game = true;
    while (restart_game) {
        game.state = ALIVE;
        game.game_number++;

        minefield_init(&game.minefield, width, height, mines);
        bool first_reveal = true;

        display_set_game(&display, &game);

        struct Tile *cur_tile = NULL; // pointer to the tile the cursor is on
        int ch; // key that was pressed
        bool continue_running_game = true;
        while (continue_running_game) {
            display_draw(&display);
            display_refresh(&display);
            cur_tile = minefield_get_tile(&game.minefield, game.minefield.cur.x, game.minefield.cur.y);
            ch = getch(); // blocks until a key is pressed
            if (ch == KEY_RESIZE) {
                display_resize(&display);
                continue;
            }

            if (display.state == HELP) {
                switch (ch) {
                    case 'H': // close help
                    case '?':
                    case 'q':
                        display_transition_game(&display);
                        break;
                }
                continue;
            }
            switch (ch) {
                case 'L': // redraw screen
                    display_resize(&display);
                    display_draw(&display);
                    display_refresh(&display);
                    break;

                case 'q': // quit
                    restart_game = false;
                    continue_running_game = false;
                    break;

                case 'r': // restart
                    continue_running_game = false;
                    break;

                case 'H': // toggle help, only checked here if not visible already
                case '?':
                    display_transition_help(&display);
                    break;

                // movement keys
                case 'h':
                case KEY_LEFT:
                    if (game.minefield.cur.x > 0)
                        game.minefield.cur.x--;
                    break;
                case 'j':
                case KEY_DOWN:
                    if (game.minefield.cur.y < game.minefield.height - 1)
                        game.minefield.cur.y++;
                    break;
                case 'k':
                case KEY_UP:
                    if (game.minefield.cur.y > 0)
                        game.minefield.cur.y--;
                    break;
                case 'l':
                case KEY_RIGHT:
                    if (game.minefield.cur.x < game.minefield.width - 1)
                        game.minefield.cur.x++;
                    break;

                case '0':
                case '^':
                    game.minefield.cur.x = 0;
                    break;
                case '$':
                    game.minefield.cur.x = game.minefield.width - 1;
                    break;
                case 'g':
                    game.minefield.cur.y = 0;
                    break;
                case 'G':
                    game.minefield.cur.y = game.minefield.height - 1;
                    break;

                case 'u': // undo
                    if (undo_flag) {
                        game_undo(&game);
                    }
                    break;

                case ' ': // reveal tile
                    if (first_reveal) {
                        // TODO: add these back lmao
                        minefield_populate(&game.minefield);
                        minefield_reveal_tile(&game.minefield, game.minefield.cur.x, game.minefield.cur.y);
                        first_reveal = false;
                        game_undo_store(&game);
                        break;
                    }
                    if (game.state != ALIVE) {
                        break;
                    }
                    if (!cur_tile->flagged && !cur_tile->visible) {
                        game_click_tile(&game, game.minefield.cur.x, game.minefield.cur.y);
                        break;
                    }
                    break;

                case 'f': // toggle flag
                    if (game.state != ALIVE) {
                        break;
                    }
                    if (!cur_tile->visible) {
                        cur_tile->flagged = !cur_tile->flagged;
                        if (cur_tile->flagged) {
                            game.minefield.placed_flags++;
                        } else {
                            game.minefield.placed_flags--;
                        }
                    }
                    break;
            }
        }
    }

    minefield_cleanup(&game.minefield);
    display_destroy(&display);
}
