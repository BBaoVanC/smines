/* https://stackoverflow.com/questions/6508461/logging-library-for-c */

#include <stdio.h>

#ifndef LOGGER_H
#define LOGGER_H

void logger(FILE *file, const char *tag, const char *message);

#endif /* LOGGER_H */
