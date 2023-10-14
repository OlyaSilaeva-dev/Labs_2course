#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int processFile(int argc, char* argv[], int* res) {

    char* filename = argv[1];
    char* flag = argv[2];
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return 1; // Не удалось открыть файл
    }

    if (strcmp(flag, "xor8") == 0) {
        if(argc != 3){
            return 4;
        }
        int result = 0;
        int byte;
        while ((byte = fgetc(file)) != EOF) {
            result ^= byte;
        }
        *res = result;
    } else if (strcmp(flag, "xor32") == 0) {
        if(argc != 3){
            return 4;
        }
        int result = 0;
        unsigned int value;
        while (fread(&value, sizeof(value), 1, file) == 1) {
            result ^= value;
        }
        *res = result;
    } else if (strcmp(flag, "mask") == 0) {
        if (argc != 4) {
            return 4; // Неправильное количество аргументов для маски
        }
        char* maskHex = argv[3];
        unsigned int maskValue;
        if (sscanf(maskHex, "%x", &maskValue) != 1) {
            return 3; // Некорректный формат маски
        }

        int count = 0;
        unsigned int currentValue;
        while (fread(&currentValue, sizeof(currentValue), 1, file) == 1) {
            //printf("%u %u\n", maskValue, currentValue);
            if ((currentValue & maskValue) == maskValue) {
                count++;
            }
        }
        *res = count;
    } else {
        return 2; // Неизвестный флаг
    }

    fclose(file);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        printf("wrong argc");
        return 1;
    }

    int res;
    int ans = processFile(argc, argv, &res);

    if (ans == 1) {
        printf("file is NULL");
        return 2;
    } else if (ans == 2) {
        printf("wrong flag");
        return 3;
    } else if (ans == 3) {
        printf("wrong mask");
        return 4;
    } else if (ans == 4) {
        printf("wrong argc");
        return 1;
    } else {
        printf("%d", res);
    }

    return 0;
}
