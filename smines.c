/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>

bool mines[10][10];
int map[10][10];
bool visible[10][10];

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
            if (group[c][r] == true) {
                //printf("I found a mine!\n");
                surrmines++;
            }
        }
    }

    r = 1;
    for (c = 0; c < 3; c = c + 2) { /* loop through 7-8 */
        if (group[c][r] == 9) {
            surrmines++;
        }
    }

    return surrmines;
}

void printtile(int x, int y) {
    if (visible[x][y]) {
        if (map[x][y] == 10) {
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
    for (y = 0; y < 10; y++) {
        for (x = 0; x < 10; x++) {
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
    for (y = 0; y < 10; y++) {
        for (x = 0; x < 10; x++) {
            printw("%d ", mines[x][y]);
        }
        printw("\n");
    }
}

bool revealtile(int x, int y) {
    if (mines[x][y] == true) {
        visible[x][y] = true;
        map[x][y] = 10;
        return false;
    } else {
        visible[x][y] = true;
        return true;
    }
}

void addmines() {
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

void setup() {
    /* zero the mines array */
    int x, y;
    for (x = 0; x < 10; x++) {
        for (y = 0; y < 10; y++) {
            mines[x][y] = 0;
        }
    }

    /* zero the visible array */
    for (x = 0; x < 10; x++) {
        for (y = 0; y < 10; y++) {
            visible[x][y] = false;
        }
    }

    addmines();

    /* calculate map */
    int s;
    for (y = 0; y < 10; y++) {
        for (x = 0; x < 10; x++) {
            if (mines[x][y] == true) {
                map[x][y] = 9;
            } else {
                s = getsurround(x, y);
                //printf("map[%i][%i] has %i surrounding mines\n", x, y, s);
                map[x][y] = s;
            }
        }
    }
}

int main() {
    setup();

    //printmines();

    int surr;
    printf("surmines(4, 4);\n");
    surr = getsurround(4, 4);
    printf("surrounding mines: %i\n", surr);

    printf("surmines(9, 9);\n");
    surr = getsurround(9, 9);
    printf("surrounding mines: %i\n", surr);


    //printf("Map:\n");
    //printmap();

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
                if (sely < 9) {
                    sely++;
                }
                break;
            case 'k':
                if (sely > 0) {
                    sely--;
                }
                break;
            case 'l':
                if (selx < 9) {
                    selx++;
                }
                break;
            case ' ':
                revealtile(selx, sely);
                break;
        }

        clear();
        printmap();
        refresh();
    }

    endwin();

}
