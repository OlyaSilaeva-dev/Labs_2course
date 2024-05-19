#include "../header.h"
#include "expression_tree.c"

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
char* get_synonym(SynonymTable* table, const char* key);
void free_table(SynonymTable* table);
status_codes replace_operations_with_synonyms(char* expression, const SynonymTable table);
status_codes calculation(char* file, const Format set, TreeNode* tree, const SynonymTable table);
status_codes Settings(char* set_file, SynonymTable* table, Format* set);

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

char* get_synonym(SynonymTable* table, const char* key) {
    for(int i = 0; i < table->count;i++) {
        if(strcmp(table->pairs[i].value, key) == 0) {
            return table->pairs[i].key;
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

status_codes replace_operations_with_synonyms(char* expression, SynonymTable table) {
    char* result = malloc(strlen(expression) + 1);
    if (!result) {
        return NO_MEMORY;
    }
    result[0] = '\0'; 

    char* token = strtok(expression, " ,();");
    while (token != NULL) {
        char* synonym = get_synonym(&table, token);
        if (synonym != NULL) {
            strcat(result, synonym);
        } else {
            strcat(result, token);
        }

        strcat(result, " ");
        token = strtok(NULL, " ,()");
    }

    strcpy(expression, result);
    free(result);
    return OK;
}

status_codes calculate_expression(uint32_t* res, char* expression, Format set, TreeNode* tree) {
    uint32_t result;
    if (strcmp(set.format, "()op") == 0) {
        result = calculationByPostfix(expression, tree);
    } else if (strcmp(set.format, "(op)") == 0) {
        char* postfix;
        infixToPostfix(expression, &postfix, tree);
        result = calculationByPostfix(postfix, tree);
        free(postfix);
    } else if (strcmp(set.format, "op()") == 0) {
        char* postfix;
        prefixToPostfix(expression, &postfix);
        result = calculationByPostfix(postfix, tree);
        free(postfix);
    } else {
        return WRONG_ELEMENT; 
    }
    (*res) = result;
    return OK;
}

void print_memory_dump(uint32_t value) {
    unsigned char *bytes = (unsigned char*)&value;
    for (int i = 0; i < sizeof(int); ++i) {
        for (int bit = 7; bit >= 0; --bit) {
            printf("%d", (bytes[i] >> bit) & 1);
        }
        printf(" ");
    }
    printf("\n");
}

void dialogue_with_user(const Format set, TreeNode* tree, const SynonymTable table) {
    while(1) {
        printf("Select an action:\n");
        printf("1: Output the value of a variable in a 16-bit system and a memory dump\n");
        printf("2: Output all variables and their values\n");
        printf("3: Change the value of a variable\n");
        printf("4: Declare a variable\n");
        printf("5: Cancel the declaration of a variable\n");
        printf("6: End the dialogue\n");
        printf("7: Shut down the interpreter\n");
    }
    
    int choice;
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        char* variable = (char*)malloc(sizeof(char) * MAX_LENGTH);
        if (!variable) return NO_MEMORY;
        printf("Enter the name of the variable: ");
        scanf("%s", variable);

        TreeNode* node = search(tree, variable);
        if (node != NULL) {
            printf("%s in the 16-point system: %x\n", variable, node->value);
            print_memory_dump(node->value);
        } else {
            printf("The %s variable was not found.", node->value);
        }
        break;

    case 2:
        printAllVariables(tree);
        break;
    
    case 3:
        
        break;
    
    case 4:
        
        break;
    
    case 5:
        
        break;
    
    case 6:
        
        break;
    
    case 7:
        
        break;
    
    default:
        break;
    }

}

status_codes calculation(char* file, const Format set, TreeNode* tree, const SynonymTable table) {
    FILE* file_calculations = fopen(file, "r");
    if(file_calculations == NULL) {
        return FILE_NOT_OPEN;
    }

    char string[BUFSIZ];
    while(fgets(string, BUFSIZ, file_calculations)) {
        
        if (string[0] == '#') {
            fgets(string,  BUFSIZ, file_calculations);
            if(strcmp(string, "BREAKPOINT") && set.isDebug) {
                dialogue_with_user(set, tree, table);
            }
            continue;
        }
        if (string[0] == '[') {
            char c;
            while(c = fgetc(file_calculations)){
                if(c == ']') {
                    break;
                }
            }
            continue;
        }

        if (string[0] == '\n') {
            continue;
        }
        char* string1 = (char*)malloc(sizeof(string));
        if (string1 == NULL) {
            return NO_MEMORY;
        }
        
        char* expression = strtok(string, ";");
        if (expression == NULL) {
            continue;
        }

        replace_operations_with_synonyms(expression, table);
        strcpy(string1, string);

        char* var = strtok(expression, " ("); 
        if (strcmp(var, "output") == 0) {
            char* var1 = strtok(NULL, ")");
            if (var1 != NULL) {
                sscanf(var1, " %s", var1);
                TreeNode* node = search(tree, var1);
                uint32_t res = node->value;
                printf("%s = %d", var1, res);
            }
        } else if (strcmp(var, "input") == 0) {
            char* var1 = strtok(NULL, ")");
            if (var1 != NULL) {
                sscanf(var1, " %s", var1);
                uint32_t val;
                printf("Enter the value for %s: ", var1);
                scanf("%d", &val);
                insert(tree, var1, val);
            }
        } else {
            if (set.isLeft) {
                printf("isLeft\n");
                uint32_t result; 
                strtok(NULL, " "); 
                char* expression1 = strtok(NULL, ";");
                if(expression1 != NULL) {
                    calculate_expression(&result, expression1, set, tree);
                    insert(tree, var, result);  
                }  
            } else {
                char* expression1 = strtok(string1, "=");          
                if(expression1 != NULL) {                    
                    uint32_t result;
                    char* var = strtok(NULL, ";");
                    sscanf(var, "%s", var);
                    calculate_expression(&result, expression1, set, tree);
                        
                    if(var != NULL) {     
                        insert(tree, var, result);             
                    }
                }  
            }
        }   
        free(string1);
    }
    freeTree(tree);
    fclose(file_calculations);
    return OK;
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
        if (string[0] == '[') {
            char c;
            while(c = fgetc(file_settings)){
                if(c == ']') {
                    break;
                }
            }
            continue;
        }
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
    SynonymTable* table = create_table();

    status_codes status = Settings(file, table, &set);
    if (status != OK) {
        fprint_err(stdout, status);
        return status;
    }

    TreeNode *root = getNode();//бор для хранения переменных
    // insert(root, "x", 2);
    // insert(root, "y", 3);
    // insert(root, "z", 4);

    // for (int i = 0; i < table->count; i++) {
    //     printf("%s %s\n", table->pairs[i].key, table->pairs[i].value);
    // }

    calculation("expression.txt", set, root, *table);

    free_table(table);
    free(set.format);

    return 0;
}
