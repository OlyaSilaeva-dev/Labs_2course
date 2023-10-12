#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int findMinBase(char *str, int* MinBase) {
    int minBase = 2;
    int len = strlen(str);

    for (int i = 0; i < len; i++) {
        if (isdigit(str[i])) {
            int digit = str[i] - '0';
            if (digit >= minBase) {
                minBase = digit + 1;
            }
        } else if (isalpha(str[i])) {
            int digit = toupper(str[i]) - 'A' + 10;
            if (digit >= minBase) {
                minBase = digit + 1;
            }
        } else {
            return 1;
        }
    }

    *MinBase = minBase;
    return 0;
}

int convertToDecimal(char *str, int base, long long *res) {
    long long result = 0;
    int len = strlen(str);

    for (int i = 0; i < len; i++) {
        int digit;
        if (isdigit(str[i])) {
            digit = str[i] - '0';
        } else {
            digit = toupper(str[i]) - 'A' + 10;
        }
        if((LLONG_MAX - digit < result * base) || (LLONG_MAX / base < result)){
            return 1;
        }
        result = result * base + digit;
    }

    *res = result;
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Wrong argc");
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        printf("file is NULL");
        return 1;
    }

    FILE *outputFile = fopen(argv[2], "w");
    if (outputFile == NULL) {
        printf("file is NULL");
        fclose(inputFile);
        return 1;
    }

    char buffer[256];

    while (fgets(buffer, sizeof(buffer), inputFile) != NULL) {
        char *token = strtok(buffer, " \t\n");
        while (token != NULL) {
            int minBase;
            int ans_minBase = findMinBase(token, &minBase);
            if(ans_minBase == 1){
                printf("Wrong base");
                return 2;
            }
            
            long long res;
            int decimalValue = convertToDecimal(token, minBase, &res);
            if(decimalValue == 1){
                printf("Overflow long long");
                return 1;
            }
            fprintf(outputFile, "%lld %d %s\n", res, minBase, token);
            token = strtok(NULL, " \t\n");
        }
    }

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
