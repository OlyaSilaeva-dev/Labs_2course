#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int GetToken(FILE *f, int *ch) {
    int c;
    while ((c = fgetc(f)) != EOF) {
        if (!isspace(c) && c != '\n') {
            *ch = c;
            return 0;
        }
    }
    *ch = EOF;
    return -1;
}

void process_file_r(const char *output_file, const char *file1, const char *file2) {
    FILE *f1 = fopen(file1, "r");
    FILE *f2 = fopen(file2, "r");
    FILE *out = fopen(output_file, "w");

    if (f1 == NULL || f2 == NULL || out == NULL) {
        perror("File Opening error");
        return;
    }

    int n1, n2;
    int result1 = GetToken(f1, &n1);
    int result2 = GetToken(f2, &n2);

    while (result1 == 0 || result2 == 0) {
        if (result1 == 0) {
            fprintf(out, "%c ", n1);
            result1 = GetToken(f1, &n1);
        }

        if (result2 == 0) {
            fprintf(out, "%c ", n2);
            result2 = GetToken(f2, &n2);
        }
    }

    fclose(f1);
    fclose(f2);
    fclose(out);
}

void process_file_a(const char *output_file, const char *input_file) {
    FILE *input = fopen(input_file, "r");
    FILE *out = fopen(output_file, "w");

    if (input == NULL || out == NULL) {
        perror("File Opening error");
        return;
    }

    int t;
    size_t cnt = 0;
    while (GetToken(input, &t) == 0) {
        cnt++;
        if (cnt % 10 == 0) {
            char buf[5];
            int coefficient = t;
            int ost, index = 0;

            while (coefficient != 0) {
                ost = coefficient % 4;
                coefficient /= 4;
                buf[index++] = ost + '0';
            }

            buf[index] = '\0';
            strrev(buf);
            fprintf(out, "%s", buf);
        } else if (cnt % 2 == 0 || cnt % 5 == 0) {
            fprintf(out, "%c", tolower(t));
        } else {
            fprintf(out, "%c", t);
        }
        fprintf(out, " ");
    }

    fclose(input);
    fclose(out);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Wrong argc\n");
        return 0;
    }

    char *flag = argv[1];
    char *file1 = argv[2];
    char *file2 = argv[3];

    if (strcmp(flag, "-r") == 0) {
        if (argc < 5) {
            fprintf(stderr, "Mistake: the output file for the flag -r is not specified\n");
            return 0;
        }

        char *output_file = argv[4];
        process_file_r(output_file, file1, file2);
    } else if (strcmp(flag, "-a") == 0) {
        if (argc < 4) {
            fprintf(stderr, "Mistake: the output file for the flag -a is not specified\n");
            return 0;
        }

        char *output_file = argv[argc - 1];
        process_file_a(output_file, file1);
    } else {
        fprintf(stderr, "Unknown flag: %s\n", flag);
        return 0;
    }

    return 0;
}
