#include "../header.h"
#define HASHSIZE 128

struct Macro {
    char *name;
    char *value;
    struct Macro *next;
};

struct Macro *hashtable[HASHSIZE];

int char2int(char num) {
    if (num >= '0' && num <= '9') {
            return num - '0';
        } else if (num >= 'A' && num <= 'Z') {
            return num - 'A' + 10;
        } else if (num >= 'a' && num <= 'z') {
            return num - 'a' + 36;
    }
}

unsigned int hash(char *str) {
    unsigned int hashval = 0;
    int base = 62;

    while (*str) {
        char c = *str++;
        int intC = char2int(c);
        hashval = hashval * base + intC;
    }

    return hashval % HASHSIZE;
}

int insert_macro(char *name, char *value) {
    unsigned int h = hash(name);

    struct Macro *new_macro = (struct Macro *)malloc(sizeof(struct Macro));
    if(new_macro == NULL) {
        return NO_MEMORY;
    }
    new_macro->name = strdup(name);
    new_macro->value = strdup(value);
    new_macro->next = hashtable[h];
    hashtable[h] = new_macro;
    return OK;
}

char *lookup_macro(char *name) {
    unsigned int h = hash(name);

    struct Macro *macro = hashtable[h];
    while (macro != NULL) {
        if (strcmp(name, macro->name) == 0) {
            return macro->value;
        }
        macro = macro->next;
    }

    return NULL;
}

int process_text(FILE *input_file) {
    char line[256];

    while (fgets(line, sizeof(line), input_file) != NULL) {
        if (line[0] == '#' && line[1] == 'd' && line[2] == 'e' && line[3] == 'f') {

            char def_name[BUFSIZ], value[BUFSIZ];
            sscanf(line, "#define %s %s", def_name, value);
            if(insert_macro(def_name, value) != OK) {
                return NO_MEMORY;
            }
        } else {
            char *token, *rest = line;
            while ((token = strtok_r(rest, " \t\n", &rest)) != NULL) {
                char *macro_value = lookup_macro(token);
                if (macro_value != NULL) {
                    printf("%s ", macro_value);
                } else {
                    printf("%s ", token);
                }
            }
        }
    }
    return OK;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        print_err(INVALID_ARGC);
        return INVALID_ARGC;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        print_err(FILE_NOT_OPEN);
        return FILE_NOT_OPEN;
    }

    int st = process_text(input_file);
    if(st != OK) {
        print_err(st);
        return st;
    }

    fclose(input_file);

    return 0;
}
