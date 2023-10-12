#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int parseNumber(char* str, int base, long long int* number) {
    long long int result = 0;
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        char c = str[i];
        long long int digit;

        if (isdigit(c)) {
            digit = c - '0';
        } else if(isalpha(c)) {
            digit = 10 + (tolower(c) - 'a');
        } else {
            return 1;
        }

        if(digit >= base){
            return 2;
        }

        result = result * base + digit;
    }

    *number = result;
    return 0;
}

void toBase(long long int number, int base) {
    char result[100]; 
    int index = 99;
    result[index] = '\0';

    while (number > 0) {
        int digit = number % base;
        if (digit < 10) {
            result[--index] = '0' + digit;
        } else {
            result[--index] = 'a' + (digit - 10);
        }
        number /= base;
    }

    printf("%s\n", &result[index]);
}

char *inputString(FILE* fp, size_t size){
    char *str;
    char* tmp;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(*str) * size); 
    if (!str) return str;
    while (EOF != (ch = fgetc(fp)) && ch != '\n') {
        str[len++] = ch;
        if (len == size) {
            tmp = realloc(str, sizeof(*str) * (size += 16));
            if (tmp != NULL) {
            str = tmp;
            return str;
            }
        }
    }
    str[len++] = '\0';
    return realloc(str, sizeof(*str) * len);
}

int main() {
    int base;
    char *input;

    input = inputString(stdin, 10);
    if (input == NULL) {
        printf("Wrong base\n");
        return 1;
    }
    base = strtol(input, NULL, 10); 
    free(input);
    if(base < 2 || base > 36){
        printf("Wrong base");
        return 1;
    }
    
    long long int maxNumber = 0;
    char maxNumberStr[100];

    if((input = inputString(stdin, 10)) != NULL && strcmp(input, "Stop") == 0){
        printf("input is empty");
        return 4;
    }

    do {
        if (strcmp(input, "Stop") == 0) {
            free(input);
            break;
        }

        long long int number;
        int answer = parseNumber(input, base, &number);

        if (answer == 1){
            printf("Invalid character in input");
            return 2;
        } else if (answer == 2){
            printf("Digit not allowed in base");
            return 3;
        }

        if (llabs(number) > llabs(maxNumber)) {
            maxNumber = number;
            strcpy(maxNumberStr, input);
        }

        free(input);
    } while ((input = inputString(stdin, 10)) != NULL); 

    printf("%lld\n", maxNumber);
    printf("9: ");
    toBase(maxNumber, 9);
    printf("18: ");
    toBase(maxNumber, 18);
    printf("27: ");
    toBase(maxNumber, 27);
    printf("36: ");
    toBase(maxNumber, 36);

    return 0;
}
