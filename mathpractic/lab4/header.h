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

typedef enum status_codes {
    OK = 0,
    INVALID_ARGC = 1,
    FILE_NOT_OPEN = -2,
    NO_MEMORY = -1,
    WRONG_INPUT = 2,
    OVERFLOWED = 3,
    LIST_IS_EMPTY = 4,
    TOO_MUCH_ITERATIONS = 5,
    BRACKETS_NOT_BALANCED = 6,
    WRONG_ELEMENT = 7,
    DIVISION_BY_ZERO = 8,
    EMPTY_OPERAND_STACK = 9,
    INVALID_FORMAT = 10,
    HEAP_IS_EMPTY = 11,
    TOO_MUCH_SYNONYMS = 12,
    WRONG_SYSTEM = 13,
} status_codes;

void fprint_err(FILE* stream, int st) {
    switch (st)
    {
    case NO_MEMORY:
        fprintf(stream, "memory allocation error\n");
        break;
    
    case INVALID_ARGC:
        fprintf(stream, "wrong argc\n");
        break;
    
    case FILE_NOT_OPEN:
        fprintf(stream, "file opening error\n");
        break;
    
    case WRONG_INPUT:
        fprintf(stream, "wrong input\n");
        break;
    
    case OVERFLOWED:
        fprintf(stream, "overflowed\n");
        break;
    
    case TOO_MUCH_ITERATIONS:
        fprintf(stream, "too much iterations\n");
        break;

    case BRACKETS_NOT_BALANCED:
        fprintf(stream, "brackets are not balanced\n");
        break;

    case WRONG_ELEMENT:
        fprintf(stream, "wrong element\n");
        break;
    
    case DIVISION_BY_ZERO:
        fprintf(stream, "division by zero\n");
        break;
    
    case EMPTY_OPERAND_STACK:
        fprintf(stream, "empty operand stack\n");
        break;

    case INVALID_FORMAT:
        fprintf(stream, "invalid expression format\n");
        break;    
    case HEAP_IS_EMPTY:
        fprintf(stream, "heap is empty\n");
    case TOO_MUCH_SYNONYMS:
        fprintf(stream, "too much synonyms\n");
    case WRONG_SYSTEM:
        fprintf(stream, "wrong system\n");
    
    default:
        break;
    }
}

#endif