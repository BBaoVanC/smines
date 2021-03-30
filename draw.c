/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include "draw.h"
#include "colornames.h"
#include "types.h"

void draw_cursor(WINDOW *win, Tile *tile, enum States game_state) {
    bool use_surrounding = false;
    bool bold_text = false;
    char *tile_text = NULL;

    if (tile->flagged) {
        if (game_state == alive) {
            tile_text = " F";
        } else if (!tile->mine) {
            tile_text = "!F";
        } else {
            tile_text = " F";
        }
    } else if (tile->visible) {
        if (tile->mine)
            tile_text = " X";
        else
            use_surrounding = true;
    } else {
        tile_text = " ?";
    }


    if (bold_text)
        wattron(win, A_BOLD);

    wattron(win, COLOR_PAIR(TILE_CURSOR));
    if (!use_surrounding)
        wprintw(win, tile_text);
    else
        if (tile->surrounding == 0)
            wprintw(win, "  ");
        else
            wprintw(win, " %i", tile->surrounding);
    wattroff(win, COLOR_PAIR(TILE_CURSOR));

    if (bold_text)
        wattron(win, A_BOLD);
}

void draw_tile(WINDOW *win, Tile *tile, bool is_cursor, bool check_flag, bool green_mines) {
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
            } else { /* if not (tile->mine) */
                if (is_cursor)
                    color_pair = COLOR_PAIR(TILE_CURSOR);
                else
                    color_pair = COLOR_PAIR(TILE_FLAG_WRONG);
                wattron(win, A_BOLD);
                wattron(win, color_pair);
                wprintw(win, "!F");
                wattroff(win, color_pair);
                wattroff(win, A_BOLD);
            }
        } else { /* if not (check_flag) */
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

    } else if (tile->visible) { /* if not (tile->flagged) */
        if (tile->mine) {
            if (is_cursor)
                color_pair = COLOR_PAIR(TILE_CURSOR);
            else if (green_mines)
                color_pair = COLOR_PAIR(TILE_MINE_SAFE);
            else
                color_pair = COLOR_PAIR(TILE_MINE);
            wattron(win, A_BOLD);
            wattron(win, color_pair);
            wprintw(win, " X");
            wattroff(win, color_pair);
            wattroff(win, A_BOLD);
        } else { /* if not (tile->mine) */
            if (is_cursor)
                color_pair = COLOR_PAIR(TILE_CURSOR);
            else
                color_pair = get_surround_color(tile->surrounding);
            wattron(win, color_pair);
            if (tile->surrounding == 0)
                wprintw(win, "  ", tile->surrounding);
            else
                wprintw(win, " %d", tile->surrounding);
            wattroff(win, color_pair);
        }

    } else { /* if neither (tile->flagged) nor (tile->visible) */
        if (is_cursor)
            color_pair = COLOR_PAIR(TILE_CURSOR);
        else
            color_pair = COLOR_PAIR(TILE_HIDDEN);
        wattron(win, color_pair);
        wprintw(win, " ?");
        wattroff(win, color_pair);
    }
}

void draw_minefield(WINDOW *win, Minefield *minefield, bool check_flag, bool green_mines, enum States game_state) {
    /* remember: multiply x by 2 because each tile is 2 cols wide */
    int cur_r = minefield->cur.row;
    int cur_c = minefield->cur.col;

    for (int y = 0; y < minefield->rows; y++) {
        for (int x = 0; x < minefield->cols; x++) {
            wmove(win, y + 1, x*2 + 1);
            draw_tile(win, &minefield->tiles[y][x], false, check_flag, green_mines);
        }
    }

    wmove(win, cur_r + 1, cur_c*2 + 1);
    draw_cursor(win, &minefield->tiles[cur_r][cur_c], game_state);
    /* draw_tile(win, &minefield->tiles[cur_r][cur_c], true, check_flag, green_mines); */
}

void draw_scoreboard(WINDOW *win, Minefield *minefield, int game_number, enum States state) {
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
