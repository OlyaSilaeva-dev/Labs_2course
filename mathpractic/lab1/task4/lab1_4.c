#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int exclude_numbers(FILE* input, FILE* output) {
    if (input == NULL || output == NULL) {
        printf("file_is_NULL");
        return 1;
    }
    char c;
    while (!feof(input)) {
        fscanf(input, "%c", &c);
        if (!isdigit(c)) {
            fputc(c, output);
        }
    }
    return 0;
}

int alfa_count(FILE* input, FILE* output) {
    if (input == NULL || output == NULL) {
        printf("file_is_NULL");
        return 1;
    }
    while (!feof(input)) {
        int cnt = 0;
        char c;
        while ((c = fgetc(input)) != '\n' && !feof(input)) {
            if (isalpha(c)) {
                cnt++;
            }
        }
        fprintf(output, "%d\n", cnt);
    }
    return 0;
}

int not_num_space_alfa(FILE* input, FILE* output) {
    if (input == NULL || output == NULL) {
        printf("file_is_NULL");
        return 1;
    }
    while (!feof(input)) {
        int cnt = 0;
        char c;
        while ((c = fgetc(input)) != '\n' && !feof(input)) {
            if (!isalnum(c) && c != ' ') {
                cnt++;
            }
        }
        fprintf(output, "%d\n", cnt);
    }
    return 0;
}

int not_num_to_ascii(FILE* input, FILE* output) {
    if (input == NULL || output == NULL) {
        printf("file_is_NULL");
        return 1;
    }
    while (!feof(input)) {
        char c;
        while ((c = fgetc(input)) != '\n' && !feof(input)) {
            if (isdigit(c)) {
                fprintf(output, "%c ", c);
            } else {
                fprintf(output, "%02X ", c);
            }
        }
        fprintf(output, "\n");
    }
    return 0;
}

int make_output_name(char* output_name, char* input_name) {
    size_t len = strlen(input_name);
    for(int i = 0; i < len + 5; i++){
        output_name[i] = 0;
    }
    char* a = input_name;
    size_t i = 0;
    while (1) {
        if (a[0] != '.' || a[1] != '.' || (a[2] != '/' && a[2] != '\\')) {
            break;
        }
        a += 3;
        i += 3;
    }

    
    strcpy(output_name, "out_");
    strncat(output_name, input_name, i);
    strncat(output_name, a, len + 4 - i);

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 3 || argc > 4) {
        printf("Invalid argc");
        return 1;
    }

    if (argv[1][0] != '-' && argv[1][0] != '/') {
         printf("Invalid flag: %c\n", argv[1][0]);
        return 1;
    }

    FILE* file_input = fopen(argv[2], "r");
    FILE* file_output = NULL;

    char flag;
    if (argv[1][1] == 'n') {
        if (argc != 4) {
            printf("Invalid argc");
            return 1;
        }
        flag = argv[1][2];
        file_output = fopen(argv[3], "w");
    } else {
        if (argc != 3) {
            printf("Invalid argc");
            return 1;
        }
        flag = argv[1][1];
        size_t len = strlen(argv[2]);
        char* output_name = (char*)malloc(len + 5);
        if(output_name == NULL){
            printf("Wrong answer");
            return 1;
        }
        make_output_name(output_name, argv[2]);
        file_output = fopen(output_name, "w");
        free(output_name);
    }

    int res;
    switch (flag) {
        case 'd':
            res = exclude_numbers(file_input, file_output);
            break;

        case 'i':
            res = alfa_count(file_input, file_output);
            break;

        case 's':
            res = not_num_space_alfa(file_input, file_output);
            break;

        case 'a':
            res = not_num_to_ascii(file_input, file_output);
            break;

        default:
            printf("Invalid flag");
            res = 1;
            break;
    }

    fclose(file_input);
    fclose(file_output);
    return res;
}

