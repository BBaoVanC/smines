/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ncurses.h>

#define MCOLS 16
#define MROWS 16
#define MINES 40

bool mines[MCOLS][MROWS];
int map[MCOLS][MROWS];
bool visible[MCOLS][MROWS];
bool flagged[MCOLS][MROWS];

static int selx = 0;
static int sely = 0;

int getsurround(int x, int y) {
    /* 1 2 3  row 1
     * 7   8  row 2
     * 4 5 6  row 3
     */

    int grpstartx = x - 1;
    int grpstarty = y - 1;
    int gx, gy;
    int group[3][3];

    for (gy = 0; gy < 3; gy++) {
        for (gx = 0; gx < 3; gx++) {
            //printf("I am setting group[%i][%i] to %i, %i\n", gx, gx, grpstartx + gx, grpstarty + gy);
            group[gx][gy] = mines[grpstartx + gx][grpstarty + gy];
        }
    }

    int ax, ay;
    /* printf("Group:\n");
    for (ay = 0; ay < 3; ay++) {
        for (ax = 0; ax < 3; ax++) {
            printf("%i ", group[ax][ay]);
        }
        printf("\n");
    } */

    int c, r;
    int surrmines = 0;
    for (r = 0; r < 3; r = r + 2) { /* loop through 1-6 */
        for (c = 0; c < 3; c++) {
            //printf("I am on position %i, %i\n", c, r);
            if (group[c][r]) {
                //printf("I found a mine!\n");
                surrmines++;
            }
        }
    }

    r = 1;
    for (c = 0; c < 3; c = c + 2) { /* loop through 7-8 */
        if (group[c][r]) {
            surrmines++;
        }
    }

    return surrmines;
}

void printtile(int x, int y) {
    if (flagged[x][y]) {
        printw("F");
    } else if (visible[x][y]) {
        if (mines[x][y]) {
            printw("X");
        } else {
            printw("%d", map[x][y]);
        }
    } else {
        printw("h");
    }
}

void printmap() {
    int x, y;
    for (y = 0; y < MROWS; y++) {
        for (x = 0; x < MCOLS; x++) {
            if ((x == selx) && (y == sely)) {
                attron(A_UNDERLINE);
                printtile(x, y);
                attroff(A_UNDERLINE);
                printw(" ");
            } else {
                printtile(x, y);
                printw(" ");
            }
        }
        printw("\n");
    }
}

void printmines() {
    int x, y;
    for (y = 0; y < MROWS; y++) {
        for (x = 0; x < MCOLS; x++) {
            printw("%d ", mines[x][y]);
        }
        printw("\n");
    }
}

bool revealtile(int x, int y) {
    if (flagged[x][y]) {
        return true;
    } else if (mines[x][y]) {
        visible[x][y] = true;
        return false;
    } else {
        /* if (map[x][y] == 0) {
            int rx, ry;
            for (rx = x - 1; rx <= x + 1; rx++) {
                for (ry = y - 1; ry <= y + 1; ry++) {
                    visible[rx][ry] = true;
                }
            }
        } else { */
        visible[x][y] = true;
    }
    return true;
}

void adddummymines() {
    mines[7][1] = true;
    mines[1][2] = true;


    mines[5][3] = true;
    mines[5][4] = true;
    mines[5][5] = true;

    mines[3][3] = true;
    mines[3][4] = true;
    mines[3][5] = true;

    mines[4][3] = true;
    mines[4][5] = true;


    mines[3][8] = true;
}

void addmines() {
    int i, m;
    int x, y;
    for (i = 0; i < MINES;) {
        x = (rand() % (MCOLS - 1 + 1));
        y = (rand() % (MCOLS - 1 + 1));
        if (!(mines[x][y])) {
            mines[x][y] = true;
            i++;
        }
    }
}

void death() {
    int x, y;
    for (x = 0; x < MCOLS; x++) {
        for (y = 0; y < MROWS; y++) {
            map[x][y] = 9;
        }
    }
}

void setup() {
    /* zero the mines array */
    int x, y;
    for (x = 0; x < MCOLS; x++) {
        for (y = 0; y < MROWS; y++) {
            mines[x][y] = 0;
        }
    }

    /* zero the visible array */
    for (x = 0; x < MCOLS; x++) {
        for (y = 0; y < MROWS; y++) {
            visible[x][y] = false;
        }
    }

    /* zero the flagged array */
    for (x = 0; x < MCOLS; x++) {
        for (y = 0; y < MROWS; y++) {
            flagged[x][y] = false;
        }
    }

    addmines();

    /* calculate map */
    int s;
    for (y = 0; y < MROWS; y++) {
        for (x = 0; x < MCOLS; x++) {
            if (!(mines[x][y])) {
                s = getsurround(x, y);
                //printf("map[%i][%i] has %i surrounding mines\n", x, y, s);
                map[x][y] = s;
            }
        }
    }
}

int main() {
    srand((unsigned) time(NULL));

    setup();

    int ch;
    printf("Starting game\n\n");

    initscr();
    keypad(stdscr, TRUE);
    noecho();
    printmap();
    refresh();
    bool keeprunning = true;
    while (keeprunning) {
        ch = getch();

        switch(ch) {
            case 'q':
                keeprunning = false;
                break;

            case 'h':
                if (selx > 0) {
                    selx--;
                }
                break;
            case 'j':
                if (sely < MROWS-1) {
                    sely++;
                }
                break;
            case 'k':
                if (sely > 0) {
                    sely--;
                }
                break;
            case 'l':
                if (selx < MCOLS-1) {
                    selx++;
                }
                break;
            case ' ':
                if (!(revealtile(selx, sely))) {
                    death();
                }
                break;
            case 'F':
            case 'f':
                flagged[selx][sely] = !flagged[selx][sely];
                break;
        }

        clear();
        printmap();
        refresh();
    }

    endwin();

}
