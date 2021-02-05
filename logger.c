/* https://stackoverflow.com/questions/6508461/logging-library-for-c */

#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void logger(FILE *file, const char *tag, const char *message) {
    time_t now;
    time(&now);
    fprintf(file, "%s [%s]: %s\n", ctime(&now), tag, message);
    fflush(file);
}
