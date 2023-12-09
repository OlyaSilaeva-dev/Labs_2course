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
    OVERFLOWED = 3,
    LIST_IS_EMPTY = 4,
    TOO_MUCH_ITERATIONS = 5,
    ELEMENT_NOT_FOUND = 6,
};

void print_err(int st) {
    switch (st)
    {
    case NO_MEMORY:
        printf("memory allocation error");
        break;
    
    case INVALID_ARGC:
        printf("wrong argc");
        break;
    
    case FILE_NOT_OPEN:
        printf("file opening error");
        break;
    
    case WRONG_INPUT:
        printf("wrong input");
        break;
    
    case OVERFLOWED:
        printf("overflowed");
        break;
    
    case TOO_MUCH_ITERATIONS:
        printf("too much iterations");
        break;
    case ELEMENT_NOT_FOUND:
        printf("element not found");
        break;
    default:
        break;
    }
}

#endif