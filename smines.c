/* smines
 * by bbaovanc
 * https://github.com/BBaoVanC/smines
 */

#include <stdio.h>

int map[10][10];

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
            printf("I am setting group[%i][%i] to %i, %i\n", gx, gx, grpstartx + gx, grpstarty + gy);
            group[gx][gy] = map[grpstartx + gx][grpstarty + gy];
        }
    }

    int ax, ay;
    printf("Group:\n");
    for (ay = 0; ay < 3; ay++) {
        for (ax = 0; ax < 3; ax++) {
            printf("%i ", group[ax][ay]);
        }
        printf("\n");
    }

    int c, r;
    int surrmines;
    for (r = 0; r < 3; r = r + 2) { /* loop through 1-6 */
        for (c = 0; c < 3; c++) {
            printf("I am on position %i, %i\n", c, r);
            if (group[c][r] == 9) {
                printf("I found a mine!\n");
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

int main() {
    int x, y;
    for (x = 0; x < 10; x++) {
        for (y = 0; y < 10; y++) {
            map[x][y] = 0;
        }
    }

    map[7][1] = 9;
    map[1][2] = 9;


    map[5][3] = 9;
    map[5][4] = 9;
    map[5][5] = 9;

    map[3][3] = 9;
    map[3][4] = 9;
    map[3][5] = 9;

    map[4][3] = 9;
    map[4][5] = 9;


    map[3][8] = 9;

    for (y = 0; y < 10; y++) {
        for (x = 0; x < 10; x++) {
            printf("%d ", map[x][y]);
        }
        printf("\n");
    }

    int surr;
    printf("surmines(4, 4);\n");
    surr = getsurround(4, 4);
    printf("surrounding mines: %i\n", surr);

}
