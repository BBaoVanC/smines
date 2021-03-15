/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include "draw.h"
#include "colornames.h"

void draw_tile(WINDOW *win, Tile *tile, bool is_cursor, bool check_flag) {
    int color_pair;
    if (tile->flagged) {
        if (check_flag) {
            if (tile->mine) {
                if (is_cursor)
                    color_pair = COLOR_PAIR(TILE_CURSOR);
                else
                    color_pair = COLOR_PAIR(TILE_FLAG);
                wattron(win, A_BOLD);
                wattron(win, color_pair);
                wprintw(win, " F");
                wattroff(win, color_pair);
                wattroff(win, A_BOLD);
            } else {
                if (is_cursor)
                    color_pair = COLOR_PAIR(TILE_CURSOR);
                else
                    color_pair = COLOR_PAIR(TILE_MINE);
                wattron(win, A_BOLD);
                wattron(win, color_pair);
                wprintw(win, "!F");
                wattroff(win, color_pair);
                wattroff(win, A_BOLD);
            }
        } else {
            if (is_cursor)
                color_pair = COLOR_PAIR(TILE_CURSOR);
            else
                color_pair = COLOR_PAIR(TILE_FLAG);
            wattron(win, A_BOLD);
            wattron(win, color_pair);
            wprintw(win, " F");
            wattroff(win, color_pair);
            wattroff(win, A_BOLD);
        }

    } else if (tile->visible) {
        if (tile->mine) {
            if (is_cursor)
                color_pair = COLOR_PAIR(TILE_CURSOR);
            else
                color_pair = COLOR_PAIR(TILE_MINE);
            wattron(win, A_BOLD);
            wattron(win, color_pair);
            wprintw(win, " X");
            wattroff(win, color_pair);
            wattroff(win, A_BOLD);
        } else {
            if (is_cursor)
                color_pair = COLOR_PAIR(TILE_CURSOR);
            else
                color_pair = getcolorforsurround(tile->surrounding);
            wattron(win, color_pair);
            if (tile->surrounding == 0)
                wprintw(win, "  ", tile->surrounding);
            else
                wprintw(win, " %d", tile->surrounding);
            wattroff(win, color_pair);
        }

    } else {
        if (is_cursor)
            color_pair = COLOR_PAIR(TILE_CURSOR);
        else
            color_pair = COLOR_PAIR(TILE_HIDDEN);
        wattron(win, color_pair);
        wprintw(win, " ?");
        wattroff(win, color_pair);
    }
}

void draw_minefield(WINDOW *win, Minefield *minefield, bool check_flag) {
    int cur_r = minefield->cur.row;
    int cur_c = minefield->cur.col;

    for (int y = 0; y < minefield->rows; y++) {
        for (int x = 0; x < minefield->cols; x++) {
            wmove(win, y + 1, x*2 + 1);
            draw_tile(win, &minefield->tiles[y][x], false, check_flag);
        }
    }

    wmove(win, cur_r + 1, cur_c*2 + 1);
    draw_tile(win, &minefield->tiles[cur_r][cur_c], true, check_flag);
}

void draw_scoreboard(WINDOW *win, Minefield *minefield, int game_number) {
    wclear(win);
    int mines = minefield->mines;
    int placed = minefield->placed_flags;
    int found_percentage = ((float)placed / (float)mines) * 100;
    mvwprintw(win, 1, 0, "Game #%i (%ix%i)", game_number, minefield->cols, minefield->rows);
    mvwprintw(win, 2, 0, "Flags: %i", placed);
    mvwprintw(win, 3, 0, "Mines: %i/%i (%i%%)", mines - placed, mines, found_percentage);
}
