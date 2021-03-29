/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

/* Draw everything */
void draw_screen();
/* Calculate and set origin_x and origin_y (global variables) */
void set_origin();
void resize_screen();
/* Reveal a tile, check for win/loss, and update game_state */
void reveal_check_state(int, int);
