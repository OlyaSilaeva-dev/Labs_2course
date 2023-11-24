#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

#define HASHSIZE 128

typedef struct Macro{
    char *name;
    char *value;
    struct Macro *next;
} Macro; 




unsigned int hash(char *str) {
    unsigned int hashval = 0;

    while (*str) {
        hashval = (hashval << 5) + (*str++); 
    }

    return hashval % HASHSIZE;
}

int insert_macro(char *name, char *value, Macro *hashtable[]) {
    unsigned int h = hash(name);

    Macro *new_macro = (Macro*)malloc(sizeof(Macro));
    if(new_macro == NULL) {
        return NO_MEMORY;
    }
    new_macro->name = strdup(name);
    new_macro->value = strdup(value);
    new_macro->next = hashtable[h];
    hashtable[h] = new_macro;
    return OK;
}

//поиск значения макроса в хэш-таблице
char *lookup_macro(char *name, Macro *hashtable[]) {
    unsigned int h = hash(name);

    Macro *macro = hashtable[h];
    while (macro != NULL) {
        if (strcmp(name, macro->name) == 0) {
            return macro->value;
        }
        macro = macro->next;
    }

    return NULL;
}

//обработка текста с заменой максросов
int process_text(FILE* input_file, Macro *hashtable[]) {
    char line[256];

    while(fgets(line, sizeof(line), input_file) != NULL) {
        char def_name[256], value[256];
        if(sscanf(line, "#define %s %s", def_name, value) == 2) {
            if(insert_macro(def_name, value, hashtable) != 0) {
                return WRONG_INPUT;
            }        
        } else {
            char *token, *rest = line;
            while ((token = strtok_r(rest, " \t\n", &rest)) != NULL) {
                char *macro_value = lookup_macro(token, hashtable);
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

int main(int argc, char* argv[]) {
    if (argc != 2) {
        return INVALID_ARGC;
    }

    Macro *hashtable[HASHSIZE];

    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        return FILE_NOT_OPEN;
    } 

    process_text(input_file, hashtable);

    fclose(input_file);

    return OK;
}