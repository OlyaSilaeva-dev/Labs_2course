#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <stdarg.h>
#include <complex.h>

enum status_codes {
    OK = 0,
    INVALID_ARGC = 1,
    FILE_NOT_OPEN = -2,
    NO_MEMORY = -1,
    WRONG_INPUT = 2,
};


#endif