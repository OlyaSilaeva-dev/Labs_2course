#include "../header.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <bor.c>

#define MAX_SYNONYMS 20

typedef struct {
    char* key;
    char* value;
} KeyValuePair;

typedef struct {
    KeyValuePair pairs[MAX_SYNONYMS];
    int count;
} SynonymTable;


typedef struct {
    char* format;
    int isLeft;
    int isDebug;
} Format;

SynonymTable* create_table();
void add_synonym(SynonymTable* table, const char* key, const char* value);
const char* get_synonym(SynonymTable* table, const char* key);
void free_table(SynonymTable* table);
status_codes replace_operations_with_synonyms(char* expression, const SynonymTable table);
status_codes calculation(char* file, const Format set, TrieNode* tree, const SynonymTable table);
status_codes Settings(char* set_file, SynonymTable* table);

SynonymTable* create_table() {
    SynonymTable* table = (SynonymTable*)malloc(sizeof(SynonymTable));
    if(table == NULL) return NULL; 
    table->count = 0;
    return table;
}

void add_synonym(SynonymTable* table, const char* key, const char* value) {
    for(int i = 0; i < table->count; i++) {
        if(strcmp(table->pairs[i].key, key) == 0) {
            free(table->pairs[i].value);
            table->pairs[i].value = strdup(value);
            return;
        }
    }
    table->pairs[table->count].key = strdup(key);
    table->pairs[table->count].value = strdup(value);
    table->count++;
}

const char* get_synonym(SynonymTable* table, const char* key) {
    for(int i = 0; i < table->count;i++) {
        if(strcmp(table->pairs[i].key, key) == 0) {
            return table->pairs[i].value;
        }
    }
    return NULL;
}

void free_table(SynonymTable* table) {
    for(int i = 0; i < table->count; i++) {
        free(table->pairs[i].key);
        free(table->pairs[i].value);
    }
    free(table);
}

status_codes replace_operations_with_synonyms(char* expression, const SynonymTable table) {
    char* result = malloc(strlen(expression)); 
    if (!result) {
        return NO_MEMORY;
    }
    result[0] = '\0'; 

    char* token = strtok(expression, " ,()");
    while (token != NULL) {
        const char* synonym = get_synonym(&table, token);
        if (synonym != NULL) {
            strcat(result, synonym);
        } else {
            strcat(result, token);
        } 

        token = strtok(NULL, " ,()");
    }

    strcpy(expression, result);
    free(result);
}

status_codes calculation(char* file, const Format set, TrieNode* tree, const SynonymTable table) {
    FILE* file_calculations = fopen(file, "r");
    if(file_calculations == NULL) {
        return FILE_NOT_OPEN;
    }

    char* string = (char*)malloc(sizeof(char) * BUFSIZ);
    while(fgets(string, BUFSIZ, file)) {
        
        if (set.isLeft) {
            char* var = strtok(string, " ");
            insert(tree, var);

            strtok(NULL, " ");
            char* expression = strtok(NULL, ";");
            replace_operations_with_synonyms(expression, table);
            
        }
    
    }
    
}

status_codes Settings(char* set_file, SynonymTable* table, Format* set) {
    FILE* file_settings = fopen(set_file, "r");
    if (file_settings == NULL) {
        return FILE_NOT_OPEN;
    }

    char string[BUFSIZ];

    while (fscanf(file_settings, "%s", string) == 1) {
        if (string[0] == '#') {
            fgets(string, sizeof(string), file_settings);
            continue;
        }
        // printf("[95] %s\n", string);
        if (string[0] == '[') {
            printf("%s\n", string);
            char c;
            while(c = fgetc(file_settings)){
                if(c == ']') {
                    break;
                }
            }
            continue;
        }
        printf("[104] %s\n", string);
        if (strcmp(string, "left=") == 0) {
            (*set).isLeft = 1;
        } else if (strcmp(string, "right=") == 0) {
            (*set).isLeft = 0;
        } else if (strcmp(string, "op()") == 0 || strcmp(string, "(op)") == 0 || strcmp(string, "()op") == 0) {
            (*set).format = (char*)malloc(strlen(string) + 1);
            if ((*set).format == NULL) {
                return NO_MEMORY;
            }
            strcpy((*set).format, string);
        } else if (strcmp(string, "--debug") == 0 || strcmp(string, "-d") == 0 || strcmp(string, "/debag") == 0) {
            (*set).isDebug = 1;
        } else {
            char string2[BUFSIZ];
            if(fscanf(file_settings, "%s", string2) == 1) {    
                    add_synonym(table, string, string2);    
                    printf("[124] %s %s\n", string, string2);
            }
        }
    }
    

    fclose(file_settings);
    return OK;
}

int main(int argc, char** argv) { // файл настроек
    // if (argc < 2) {
    //     fprint_err(stdout, INVALID_ARGC);
    //     return INVALID_ARGC;
    // }

    char file[] = "data.txt";
    Format set;
    SynonymTable* table = create_table();;

    status_codes status = Settings(file, table, &set);
    if (status != OK) {
        fprint_err(stdout, status);
        return status;
    }

    TrieNode *root = getNode();//бор для хранения переменных

    for (int i = 0; i < table->count; i++) {
        printf("%s %s\n", table->pairs[i].key, table->pairs[i].value);
    }

    if(table) {
        free(table);
    }
    free(set.format);





    return 0;
}
