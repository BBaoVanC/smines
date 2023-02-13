/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#ifndef SMINES_DIFFICULTIES_H
#define SMINES_DIFFICULTIES_H

/* default difficulty levels, so you don't
 * have to manually look them up and
 * enter them */
#ifdef EASY
    #define MROWS 9
    #define MCOLS 9
    #define MINES 10
#endif

#ifdef INTERMEDIATE
    #define MROWS 16
    #define MCOLS 16
    #define MINES 40
#endif

#ifdef HARD
    #define MROWS 16
    #define MCOLS 30
    #define MINES 99
#endif

#ifdef SUPER_EASY
    #define MROWS 10
    #define MCOLS 20
    #define MINES 10
#endif

#endif
