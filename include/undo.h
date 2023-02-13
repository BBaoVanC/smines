/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#ifndef SMINES_UNDO_H
#define SMINES_UNDO_H

/* Copy the minefield and game state to prepare for an undo */
void copy_undo(void);
/* Basically just swap the undo state with the current state
 * This means you can undo one action, and pressing undo again
 * will "undo the undo" */
void undo(void);

#endif
