/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include "draw.h"
#include "colornames.h"
#include "types.h"

/*
 * draw_tile_color - draw_tile, but with color
 * inputs:
 *  WINDOW *win: the window to draw into
 *  Tile *tile: the tile to draw
 *  Game_State game_state: the current game state
 */
void draw_tile_color(WINDOW *win, Tile *tile, Game_State game_state) {
    int color;

    if (tile->flagged) {

        if (game_state == victory)
            color = COLOR_PAIR(TILE_MINE_SAFE);
        else if ((game_state == dead) && (!tile->mine))
            color = COLOR_PAIR(TILE_FLAG_WRONG);
        else
            color = COLOR_PAIR(TILE_FLAG);

    } else if (tile->visible) {

        if (tile->mine)
            color = COLOR_PAIR(game_state == victory ? TILE_MINE_SAFE : TILE_MINE);
        else
            color = get_surround_color(tile->surrounding);

    } else {
        color = COLOR_PAIR(TILE_HIDDEN);
    }

    draw_tile(win, tile, color, game_state);
}

/*
 * draw_tile - draw a tile with a specified color
 * inputs:
 *  WINDOW *win: the window to draw in
 *  Tile *tile: the tile to draw
 *  int color: ncurses color pair to use
 *  Game_State game_state: the current game state
 */
void draw_tile(WINDOW *win, Tile *tile, int color, Game_State game_state) {
    wattron(win, color);

    if (tile->flagged) {
        wattron(win, A_BOLD);

        if ((game_state == dead) && (!tile->mine))
            wprintw(win, "!F");
        else
            wprintw(win, " F");

        wattroff(win, A_BOLD);
    } else if (tile->visible) {
        if (tile->mine) {
            wattron(win, A_BOLD);
            wprintw(win, " X");
            wattroff(win, A_BOLD);
        } else {
            if (tile->surrounding == 0)
                wprintw(win, "  ");
            else
                wprintw(win, " %i", tile->surrounding);
        }
    } else {
        wprintw(win, " ?");
    }

    wattroff(win, color);
}

/* draw_minefield - run draw_tile_color on an entire minefield
 * inputs:
 *  WINDOW *win: the window to draw into
 *  Minefield *minefield: the minefield to draw
 *  Game_State game_state: the current gqme state
 */
void draw_minefield(WINDOW *win, Minefield *minefield, Game_State game_state) {
    /* remember: multiply x by 2 because each tile is 2 cols wide */
    int cur_r = minefield->cur.row;
    int cur_c = minefield->cur.col;

    for (int y = 0; y < minefield->rows; y++) {
        for (int x = 0; x < minefield->cols; x++) {
            wmove(win, y + 1, x * 2 + 1);
            draw_tile_color(win, &minefield->tiles[y][x], game_state);
        }
    }

    wmove(win, cur_r + 1, cur_c * 2 + 1);
    draw_tile(win, &minefield->tiles[cur_r][cur_c], COLOR_PAIR(TILE_CURSOR), game_state);
}

/*
 * draw_scoreboard - draw the scoreboard
 * inputs:
 *  WINDOW *win: the window to draw in
 *  Minefield *minefield: the current minefield (used for calculations)
 *  int game_number: the current game number
 *  Game_State state: the current game state
 */
void draw_scoreboard(WINDOW *win, Minefield *minefield, int game_number, Game_State state) {
    wclear(win); /* if we don't clear, then if the new text is shorter than the old
                    text, characters are left on screen */
    int mines = minefield->mines;
    int placed = minefield->placed_flags;
    int found_percentage = ((float)placed / (float)mines) * 100;
    mvwprintw(win, 1, 0, "Game #%i (%ix%i)", game_number, minefield->cols, minefield->rows);
    mvwprintw(win, 2, 0, "Flags: %i", placed);
    mvwprintw(win, 3, 0, "Mines: %i/%i (%i%%)", mines - placed, mines, found_percentage);

    switch (state) { /* draw the top line */
        case alive:
            wattron(win, A_BOLD);
            mvwprintw(win, 0, 0, "Press H or ? for help");
            wattroff(win, A_BOLD);
            break;

        case victory:
            wattron(win, A_BOLD);
            wattron(win, COLOR_PAIR(MSG_WIN));
            mvwprintw(win, 0, 0, "YOU WIN!");
            wattroff(win, COLOR_PAIR(MSG_WIN));
            wattroff(win, A_BOLD);
            break;

        case dead:
            wattron(win, A_BOLD);
            wattron(win, COLOR_PAIR(MSG_DEATH));
            mvwprintw(win, 0, 0, "YOU DIED!");
            wattroff(win, COLOR_PAIR(MSG_DEATH));
            wattroff(win, A_BOLD);
            break;
    }

    wrefresh(win);
}
