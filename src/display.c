#include "display.h"

#include "colornames.h"
#include "game.h"
#include "minefield.h"

#include <ncurses.h>

#include <stddef.h>
#include <stdlib.h>

static const int SCOREBOARD_ROWS = 4;
static const char helptxt[] =
    "H or ?: view this help page\n"
    "L: redraw screen (just in case)\n"
    "\n"
    "q: quit\n"
    "r: new game\n"
    "space: reveal tile under cursor\n"
    "f: place flag\n"
    "u: undo last move (undoing a second time will 'undo the undo')\n"
    "\n"
    "Use hjkl or arrow keys to move\n"
    "0 or ^: jump to left side\n"
    "$: jump to right side\n"
    "g: jump to top side\n"
    "G: jump to bottom side\n"
;

/* set the correct starting position to center the game in the terminal */
/* reads from ncurses to figure that out */
static void display_update_origin(struct Display *display) {
    int scr_rows, scr_cols;
    getmaxyx(stdscr, scr_rows, scr_cols);
    /* add 1 col/row per side for each border, so 2 rows and 2 cols for all 4 borders */
    int width = display->game->minefield.width * 2 + 2;
    int height = display->game->minefield.height + SCOREBOARD_ROWS * 2;

    display->origin.x = (scr_cols - width) / 2;
    display->origin.y = (scr_rows - height) / 2;

    /* prevent from starting off screen in top-left direction */
    /* TODO: is there a nicer way to do these checks */
    if (display->origin.x < 0) {
        display->origin.x = 0;
    }
    if (display->origin.y < 0) {
        display->origin.y = 0;
    }
}
/* destroy_win - delete a window without leaving artifacts on screen
 * Source: https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/windows.html
 */
static void destroy_win(WINDOW *local_win) {
    werase(local_win);
    /* box(local_win, ' ', ' '); : This won't produce the desired
     * result of erasing the window. It will leave it's four corners
     * and so an ugly remnant of window.
     */
    wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    /* The parameters taken are
     * 1. win: the window on which to operate
     * 2. ls: character to be used for the left side of the window
     * 3. rs: character to be used for the right side of the window
     * 4. ts: character to be used for the top side of the window
     * 5. bs: character to be used for the bottom side of the window
     * 6. tl: character to be used for the top left corner of the window
     * 7. tr: character to be used for the top right corner of the window
     * 8. bl: character to be used for the bottom left corner of the window
     * 9. br: character to be used for the bottom right corner of the window
     */
    delwin(local_win);
}
static void display_make_windows(struct Display *display) {
    display->scoreboard = newwin(SCOREBOARD_ROWS, display->game->minefield.width * 2, display->origin.y, display->origin.x);

    /* add 2 for borders */
    display->minefield = newwin(display->game->minefield.height + 2, display->game->minefield.width * 2 + 2, display->origin.y + SCOREBOARD_ROWS, display->origin.x);

    display->too_small_popup = newwin(2, COLS, 0, 0);
}
static void display_set_min_size(struct Display *display) {
    /* check if terminal is too small */
    display->min_width = display->game->minefield.width * 2 + 2;
    display->min_height = SCOREBOARD_ROWS + display->game->minefield.height + 2; /* add 2 for borders */
    if (COLS < display->min_width || LINES < display->min_height) {
        display->too_small = true;
    } else {
        display->too_small = false;
    }
}
/* recalculate everything if the terminal is resized */
/* TODO: maybe store Minefield somewhere in Display so it's not an arg */
/* TODO: should this be `display_reset` instead? */
void display_resize(struct Display *display) {
    display->erase_needed = true;

    destroy_win(display->scoreboard);
    destroy_win(display->minefield);
    destroy_win(display->too_small_popup);

    endwin(); /* make ncurses recalculate stuff like global vars LINES and COLS */
    display_set_min_size(display);
    display_update_origin(display);
    display_make_windows(display);
}

bool display_init(struct Display *display) {
    /* ncurses setup */
    initscr();
    if (!has_colors()) {
        endwin();
        printf("smines requires color support in your terminal to work properly.\n");
        return false;
    }
    keypad(stdscr, TRUE); /* arrow keys */
    noecho(); /* don't show letter on key press */
    curs_set(0); /* make cursor invisible */
    start_color();
    use_default_colors(); /* allows extra colors such as terminal background color */

    /* set up color pairs */
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

    *display = (struct Display){0};
    return true;
}

void display_set_game(struct Display *display, struct Game *game) {
    display->game = game;
    display_set_min_size(display);
    display_update_origin(display);
    display_make_windows(display);
}

void display_destroy(struct Display *display) {
    endwin();
}

/* get color pair needed to draw a tile with a specific amount of surrounding mines */
static int get_surround_color(int surrounding) {
    if (surrounding == 0) {
        return COLOR_PAIR(10);
    } else if ((surrounding <= 8) && (surrounding >= 1)) { /* 1 <= surrounding <= 8 */
        return COLOR_PAIR(surrounding);
    } else {
        return COLOR_PAIR(100);
    }
}
static void display_draw_tile_text(struct Display *display, struct Tile *tile, int x, int y) {
    WINDOW *win = display->minefield;
    wmove(win, y + 1, x * 2 + 1); /* add 1 because of border? TODO: verify this */
    if (tile->flagged) {
        wattron(win, A_BOLD);
        if (display->game->state == DEAD && !tile->mine) {
            wprintw(win, "!F");
        } else {
            wprintw(win, " F");
        }
        wattroff(win, A_BOLD);
    } else if (tile->visible) {
        if (tile->mine) {
            wattron(win, A_BOLD);
            wprintw(win, " X");
            wattroff(win, A_BOLD);
        } else {
            if (tile->surrounding == 0) {
                wprintw(win, "  ");
            } else {
                wprintw(win, " %i", tile->surrounding);
            }
        }
    } else {
        wprintw(win, " ?");
    }
}
static void display_draw_tile(struct Display *display, struct Tile *tile, int x, int y) {
    int color;
    /* get the color pair to draw with */
    if (tile->flagged) {
        if (display->game->state == VICTORY) {
            color = COLOR_PAIR(TILE_MINE_SAFE);
        } else if ((display->game->state == DEAD) && (!tile->mine)) {
            color = COLOR_PAIR(TILE_FLAG_WRONG);
        } else {
            color = COLOR_PAIR(TILE_FLAG);
        }
    } else if (tile->visible) {
        if (tile->mine) {
            color = COLOR_PAIR(display->game->state == VICTORY ? TILE_MINE_SAFE : TILE_MINE);
        } else {
            color = get_surround_color(tile->surrounding);
        }
    } else {
        color = COLOR_PAIR(TILE_HIDDEN);
    }
    wattron(display->minefield, color);
    display_draw_tile_text(display, tile, x, y);
    wattroff(display->minefield, color);
}

static void display_draw_minefield(struct Display *display) {
    int y, x;
    for (y = 0; y < display->game->minefield.height; y++) {
        for (x = 0; x < display->game->minefield.width; x++) {
            display_draw_tile(display, minefield_get_tile(&display->game->minefield, x, y), x, y);
        }
    }

    /* draw the cursor */
    int cur_x = display->game->minefield.cur.x;
    int cur_y = display->game->minefield.cur.y;
    wmove(display->minefield, cur_y + 1, cur_x * 2 + 1); /* add 1 because of border? TODO: verify this */
    wattron(display->minefield, COLOR_PAIR(TILE_CURSOR));
    display_draw_tile_text(display, minefield_get_tile(&display->game->minefield, cur_x, cur_y), cur_x, cur_y);
    wattroff(display->minefield, COLOR_PAIR(TILE_CURSOR));
    
    wborder(display->minefield, 0, 0, 0, 0, 0, 0, 0, 0);
}

static void display_draw_scoreboard(struct Display *display) {
    WINDOW *win = display->scoreboard;
    werase(win); /* if we don't clear, and the new text is shorter than the old text, characters are left on screen */
    size_t mines = display->game->minefield.mines;
    size_t placed = display->game->minefield.placed_flags;
    int found_percentage = ((float)placed / (float)mines) * 100;
    mvwprintw(win, 1, 0, "Game #%i (%lix%li)", display->game_number, display->game->minefield.width, display->game->minefield.height);
    mvwprintw(win, 2, 0, "Flags: %li", placed);
    mvwprintw(win, 3, 0, "Mines: %li/%li (%i%%)", mines - placed, mines, found_percentage);

    /* TODO: somehow this doesnt work on first frame until keypress when window is close to not fitting */
    switch (display->game->state) { /* draw the top line */
        case ALIVE:
            wattron(win, A_BOLD);
            mvwprintw(win, 0, 0, "Press ? for help");
            wattroff(win, A_BOLD);
            break;
        case VICTORY:
            wattron(win, A_BOLD);
            wattron(win, COLOR_PAIR(MSG_WIN));
            mvwprintw(win, 0, 0, "YOU WIN!");
            wattroff(win, COLOR_PAIR(MSG_WIN));
            wattroff(win, A_BOLD);
            break;
        case DEAD:
            wattron(win, A_BOLD);
            wattron(win, COLOR_PAIR(MSG_DEATH));
            mvwprintw(win, 0, 0, "YOU DIED!");
            wattroff(win, COLOR_PAIR(MSG_DEATH));
            wattroff(win, A_BOLD);
            break;
        default:
            abort();
    }
}

void display_draw(struct Display *display) {
    if (display->erase_needed) {
        erase();
        display->erase_needed = false;
    }
    if (display->too_small) {
        wmove(display->too_small_popup, 0, 0);
        /* TODO: make a window to display this so it overlays */
        wprintw(display->too_small_popup, "Please make your terminal at least %i cols by %i rows\n", display->min_width, display->min_height);
        wprintw(display->too_small_popup, "Current size: %i cols by %i rows", COLS, LINES);
        /*wrefresh(display->too_small_popup); */
        return;
    }

    switch (display->state) {
        case HELP:
            mvprintw(0, 0, helptxt);
            break;
        case GAME:
            display_draw_minefield(display);
            display_draw_scoreboard(display);
            break;
        default:
            abort();
    }
}

void display_refresh(struct Display *display) {
    refresh();
    wrefresh(display->scoreboard);
    wrefresh(display->minefield);
    wrefresh(display->too_small_popup);
}

void display_transition_help(struct Display *display) {
    display->state = HELP;
    display->erase_needed = true;
}

void display_transition_game(struct Display *display) {
    display->state = GAME;
    display->erase_needed = true;
}
