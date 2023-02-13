/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

/* Draw everything */
void draw_screen(void);
/* Calculate and set origin_x and origin_y (global variables) */
void set_origin(void);
void resize_screen(void);
/* Reveal a tile, check for win/loss, and update game_state */
void reveal_check_state(int, int);
