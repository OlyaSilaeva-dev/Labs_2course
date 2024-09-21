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
    int base_assign;
    int base_input;
    int base_output;
} Format;

SynonymTable* create_table();
status_codes add_synonym(SynonymTable* table, const char* key, const char* value);
char* get_synonym(SynonymTable* table, const char* key);
void free_table(SynonymTable* table);
status_codes replace_operators_with_synonyms(char* expression, const SynonymTable table, Format set);
status_codes toDecimal(char *value, int system, uint32_t* result);
status_codes calculation(char* file, const Format set, TreeNode* tree, const SynonymTable table);
status_codes Settings(char* set_file, SynonymTable* table, Format* set);

SynonymTable* create_table() {
    SynonymTable* table = (SynonymTable*)malloc(sizeof(SynonymTable));
    if(table == NULL) return NULL; 
    table->count = 0;
    return table;
}

status_codes add_synonym(SynonymTable* table, const char* key, const char* value) {
    for(int i = 0; i < table->count; i++) {
        if(strcmp(table->pairs[i].key, key) == 0) {
            free(table->pairs[i].value);
            table->pairs[i].value = strdup(value);
            return OK;
        }
    }
    if (table->count < MAX_SYNONYMS) {   
        table->pairs[table->count].key = strdup(key);
        table->pairs[table->count].value = strdup(value);
        table->count++;
    } else {
        return TOO_MUCH_SYNONYMS;
    }
    return OK;
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

status_codes uintToString(uint32_t val, char* string) {
    char* tmp = (char*)malloc(sizeof(char) * MAX_LENGTH);
    if (!tmp) return NO_MEMORY;
    int ind = 0;

    do {
        tmp[ind++] = '0' + (val % 10);
        val /= 10;
    } while (val > 0);

    for (int i = 0; i < ind; i++) {
        string[i] = tmp[ind - i - 1];
    }
    string[ind] = '\0';

    free(tmp);
    return OK;
}

status_codes replace_operations_in_decimal(char* expression, Format set, TreeNode* tree){
    
    printf("[98]%s\n", expression);
    char* tmp = (char*)malloc(sizeof(char) * BUFSIZ);

    if (tmp == NULL) {
        return NO_MEMORY;
    }
    printf("[101]%s\n", expression);
    tmp[0] = '\0';
    char* token = strtok(expression, " ");
    while (token != NULL) {
        printf("[104] %s\n", token);
        if (isOperator(token, 0)) {
            strcat(tmp, token);
        } else {
            TreeNode* node = search(tree, token);
            if(node != NULL) {
                printf("[110] %s", token);
                strcat(tmp, token);
            } else {
                printf("[116] %s\n", token);
                uint32_t dec; 
                status_codes st = toDecimal(token, set.base_assign, &dec);
                if (st != OK) {
                    free(tmp);
                    return st;
                }
                char* string = (char*)malloc(sizeof(char) * MAX_LENGTH);
                if (!string) {
                    free(tmp);
                    return NO_MEMORY;
                }
                status_codes st1 = uintToString(dec, string);
                if (st1 != OK) {
                    free(tmp);
                    return st1;
                }
                strcat(tmp, string);
                free(string);
            }
        }
        strcat(tmp," ");
        token = strtok(NULL, " ");
    }
    strcat(tmp, "\0");
    strcpy(expression, tmp);
    free(tmp);
    return OK;
}



status_codes replace_operators_with_synonyms(char* expression, SynonymTable table, Format set) {
    char* result = (char*)malloc(strlen(expression) + 1);
    if (!result) {
        return NO_MEMORY;
    }
    result[0] = '\0'; 

    char* token = strtok(expression, " ,();\0");
    while (token != NULL) {
        printf("%s\n", token);
        char* synonym = get_synonym(&table, token);
        size_t result_len = strlen(result);
        size_t synonym_len = synonym ? strlen(synonym) : strlen(token);
        char* tmp = (char*)realloc(result, result_len + synonym_len + 2);
        result = tmp;
        
        if (!result) {
            free(result); // Free the original buffer if realloc failed
            return NO_MEMORY;
        }
        if (synonym != NULL) {
            printf("[166] %s %s\n", token, synonym);
            strcat(result, synonym);
        } else {
            printf("[169] %s\n", token);
            strcat(result, token);
        }
        
        token = strtok(NULL, " ,();\0");
        if (token != NULL) {
            strcat(result, " ");
        }
    }

    printf("%s\n", result);
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
        status_codes st1 = infixToPostfix(expression, &postfix, tree);
        if (st1 != OK) {
            return st1;
        }
        result = calculationByPostfix(postfix, tree);
        free(postfix);
    } else if (strcmp(set.format, "op()") == 0) {
        char* postfix;
        status_codes st2 = prefixToPostfix(expression, &postfix);
        if(st2 != OK) {
            return st2;
        }
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

status_codes charToDigit(char c, uint32_t* num) {    
    if ('0' <= c && c <= '9') {
        (*num) = c - '0';
    } else if ('A' <= c && c <= 'V') {
        (*num) = (c - 'A') + 10;
    } else {    
        printf("%c\n", c);
        return WRONG_ELEMENT;    
    }
    return OK;
}

status_codes toDecimal(char *value, int system, uint32_t* result) {
    size_t len = strlen(value);
    uint32_t dec = 0;
    int st1 = 0;
    for(int i = len - 1; i >= 0; i--) {
        uint32_t val;
        if(charToDigit(value[i], &val) == OK) {
           dec += val * st(system, st1);
           st1++; 
        } else {
            return WRONG_ELEMENT;
        }
    }
    (*result) = dec; 
    return OK;
}


status_codes digitToChar(uint32_t num, char* c) {    
    if (0 <= num && num <= 9) {
        *c = (char)('0' + num);
    } else {
        *c = (char)('A' + num - 10);
    }
    return OK;
}

status_codes reverse(char* string) {
    size_t len = strlen(string);
    char* string1 = (char*)malloc(sizeof(char) * (len + 1));
    if(!string1) return NO_MEMORY;
    for(int i = len - 1; i >= 0; i--) {
        string1[len - i - 1] = string[i];
    }
    string1[len] = '\0';

    strcpy(string, string1);
    free(string1);
    return OK;
}

status_codes toTheSystem(uint32_t value, int system, char* result) {
    char* res = (char*)malloc(sizeof(char) * MAX_LENGTH);
    if(!res) return NO_MEMORY;
    
    int i = 0;
    while (value >= system) {
        int ost = value % system;
        char c;
        digitToChar(ost, &c);
        res[i++] = c; 
        value /= system; 
    }
    char c;
    digitToChar(value, &c);
    res[i++] = c;

    res[i] = '\0';
    
    if(reverse(res) != OK) {
        free(res);
        return NO_MEMORY;
    }
    strcpy(result, res);
    free(res);
    return OK;
}

uint32_t romanToInt(char c) {
    switch (c) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
        default: return 0;
    }
}

uint32_t romanToDec(const char *roman) {
    uint32_t result = 0;
    int length = strlen(roman);
    for (int i = 0; i < length; i++) {
        uint32_t current = romanToInt(roman[i]);
        uint32_t next = (i + 1 < length) ? romanToInt(roman[i + 1]) : 0;

        if (next > current) {
            result -= current;
        } else {
            result += current;
        }
    }
    return result;
}

uint32_t zeckendorfToDecimal(const char *zeckendorf) {
    uint32_t decimal = 0;
    uint32_t fib1 = 1;
    uint32_t fib2 = 1; 

    for (int i = 0; zeckendorf[i] != '\0'; ++i) {  
        if (zeckendorf[i] == '1') {
            decimal += fib1;
        }
        uint32_t tmp = fib1 + fib2;
        fib1 = fib2;
        fib2 = tmp;
    }

    return decimal;
}


void printAllVariablesHelper(TreeNode *node, char *buffer, int depth, Format set) {
    if (node == NULL) return;

    if (node->isEndOfkey) {
        buffer[depth] = '\0';
        if (set.base_output != 10) {
            char ch[BUFSIZ];
            toTheSystem(node->value, set.base_output, ch);
            printf("%s: %s\n", buffer, ch);
        } else {
            printf("%s: %u\n", buffer, node->value);    
        }
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            buffer[depth] = indexToChar(i);
            printAllVariablesHelper(node->children[i], buffer, depth + 1, set);
        }
    }
}

void printAllVariables(TreeNode *root, Format set) {
    char buffer[MAX_SIZE];
    printAllVariablesHelper(root, buffer, 0, set);
}

status_codes dialogue_with_user(const Format set, TreeNode* tree, const SynonymTable table, int* flag) {
    while(1) {
        printf("Select an action:\n");
        printf("1: Output the value of a variable in a 16-bit system and a memory dump\n");
        printf("2: Output all variables and their values\n");
        printf("3: Change the value of a variable\n");
        printf("4: Declare a variable\n");
        printf("5: Cancel the declaration of a variable\n");
        printf("6: End the dialogue\n");
        printf("7: Shut down the interpreter\n");
    
        int choice;
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter the name of the variable: ");
            char* variable1 = (char*)malloc(sizeof(char) * MAX_LENGTH);
            if (!variable1) {
                fprint_err(stdout, NO_MEMORY);
                break;
            }
            scanf("%s", variable1);

            TreeNode* node = search(tree, variable1);
            if (node != NULL) {
                printf("%s in the 16-point system: %x\n", variable1, node->value);
                print_memory_dump(node->value);
            } else {
                printf("The variable %s was not found.\n", variable1);
            }
            free(variable1);
            break;

        case 2:
            printAllVariables(tree, set);
            break;
        
        case 3:
            printf("Enter the name of the variable: ");
            char* variable2 = (char*)malloc(sizeof(char) * MAX_LENGTH);
            if (!variable2) {
                fprint_err(stdout, NO_MEMORY);
                break;
            }
            
            scanf("%s", variable2);

            char* value = (char*)malloc(sizeof(char) * MAX_LENGTH);
            if (!value) {
                free(variable2);
                fprint_err(stdout, NO_MEMORY);
                break;
            }
            
            TreeNode* node1 = search(tree, variable2);
            if (node1 == NULL) {
                printf("the variable %s not found\n", variable2);
            } else {
                printf("Enter the value of the variable: ");
                scanf("%s", value);
                uint32_t res;
                toDecimal(value, 16, &res);
                
                node1->value = res;
            }
            free(variable2);
            free(value);
            break;
        case 4:            
            printf("Enter the name of the variable: ");
            char* variable3 = (char*)malloc(sizeof(char) * MAX_SIZE);
            if (!variable3) {
                fprint_err(stdout, NO_MEMORY);
                break;
            }
            scanf("%s", variable3);           
            TreeNode* node2 = search(tree, variable3);
            if(node2 != NULL) {
                printf("the variable %s already exists\n", variable3);
            } else {
                int format;
                while (1) {
                    printf("Select the format for entering the value:\n");
                    printf("1: in the Zeckendorf representation\n");
                    printf("2: written in Roman numerals\n");
                    scanf("%d", &format);

                    if(format == 1 || format == 2) {
                        char* value1 = (char*)malloc(sizeof(char) * MAX_LENGTH);
                        if(!value1) {
                            free(variable3);
                            fprint_err(stdout, NO_MEMORY);
                            break;
                        }
                        scanf("%s", value1);

                        uint32_t val;
                        if(format == 1){
                            val = zeckendorfToDecimal(value1);
                        } else if (format == 2) {
                            val = romanToDec(value1);
                        }
                        
                        insert(tree, variable3, val);
                        free(value1);
                        break;
                    } else {
                        printf("Wrong format\n");
                    }
                }
            }            
            
            free(variable3);
            break;
        
        case 5:
            printf("Enter the variable:\n");
            char* variable4 = (char*)malloc(sizeof(char) * MAX_SIZE);
            if (!variable4) {
                fprint_err(stdout, NO_MEMORY);
                break;
            }
            scanf("%s", variable4);
            TreeNode* node3 = search(tree, variable4);
            if (node3 == NULL) {
                printf("The variable %s was not found\n", variable4);
            } else {
                deleteKey(tree, variable4);
            }

            free(variable4);
            break;
        
        case 6:
            return OK;
            break;
        
        case 7:
            (*flag) = 0;
            return OK;
            break;
        
        default:
            break;
        }
    }
}

status_codes calculation(char* file, const Format set, TreeNode* tree, const SynonymTable table) {
    FILE* file_calculations = fopen(file, "r");
    if(!file_calculations) return FILE_NOT_OPEN;

    char string[BUFSIZ];

    while(fgets(string, BUFSIZ, file_calculations)) {   
        if (string[0] == '#') {
            string[strcspn(string, "\n")] = 0;
            if((strcmp(string, "#BREAKPOINT") == 0) && set.isDebug) {
                int flag = 1;
                status_codes st = dialogue_with_user(set, tree, table, &flag);
                if(st != OK || flag == 0) {
                    fclose(file_calculations);
                    return st;
                }
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
        char* string1 = (char*)malloc(sizeof(string) + 1);
        if (string1 == NULL) {
            fclose(file_calculations);
            return NO_MEMORY;
        }

        strcpy(string1, string);       
        char* expression = strtok(string, ";");
        if (expression == NULL) {
            free(string1);
            continue;
        }

        status_codes st1 = replace_operators_with_synonyms(expression, table, set);

        if(st1 != OK) {
            free(string1);
            fclose(file_calculations);
            return st1;
        }

        char* expression1 = (char*) malloc(strlen(expression)*sizeof(char) + 1); // для right
        if(expression1 == NULL) {
            free(string1);
            fclose(file_calculations);
            return NO_MEMORY;
        }

        strcpy(expression1, expression);
        printf("[588] %s\n", expression1);
        char* var = strtok(expression, " ("); 

        if (var != NULL && strcmp(var, "output") == 0) {
            char* var1 = strtok(NULL, ")");
            if (var1 != NULL) {
                sscanf(var1, " %s", var1);
                TreeNode* node = search(tree, var1);
                if (node != NULL) { 
                    uint32_t dec = node->value;
                    char* res = (char*)malloc(sizeof(char) * MAX_LENGTH);
                    if (!res) {
                        free(string1);
                        fclose(file_calculations);
                        free(expression1);
                        return NO_MEMORY;
                    }
                    int status = toTheSystem(dec, set.base_output, res);
                    if (status != OK) {
                        free(string1);
                        fclose(file_calculations);
                        free(expression1);
                        return status;
                    }
                    printf("%s = %s\n", var1, res);
                    free(res);
                }
            }
        } else if (var != NULL && strcmp(var, "input") == 0) {
            char* var1 = strtok(NULL, ")");
            if (var1 != NULL) {
                sscanf(var1, " %s", var1);
                char* val = (char*)malloc(sizeof(char) * MAX_LENGTH);
                if (!val) {
                    free(string1);
                    fclose(file_calculations);
                    free(expression1);
                    return NO_MEMORY;
                }
                scanf("%s", val);
                uint32_t res;
                status_codes status = toDecimal(val, set.base_output, &res);
                if (status != OK) {
                    free(string1);
                    fclose(file_calculations);
                    free(expression1);
                    return status;
                }
                insert(tree, var1, res);
            }
        } else {
            if (set.isLeft) {
                uint32_t result; 
                strtok(NULL, " "); //пропускаем =
                char* tok = strtok(NULL, ";");

                if(tok != NULL) {
                    char expr[BUFSIZ];
                    strcpy(expr, tok);
                    if(set.base_assign != 10) {
                        status_codes st2 = replace_operations_in_decimal(expr, set, tree);
                        if (st2 != OK) {
                            free(string1);
                            fclose(file_calculations);
                            free(expression1);
                            return st2;
                        }
                    }

                    status_codes st_calc = calculate_expression(&result, expr, set, tree);
                    if (st_calc != OK) {
                        free(string1);
                        fclose(file_calculations);
                        free(expression1);
                        return st_calc;
                    }
                    insert(tree, var, result);  
                }  
            } else {
                
                char* tok = strtok(expression1, "=");
             
                printf("[669] %s\n", tok);          
                if(tok != NULL) {                    
                    uint32_t result;
                    char* var_name = strtok(NULL, ";");
                    if (var_name != NULL) {
                        while (isspace(*var_name)) {
                            var_name++;
                        }
                    }
                    
                    printf("[673] %s\n", var_name);
                    char expr[BUFSIZ];
                    strcpy(expr, tok);
                    printf("[684] %s\n", expr);
                    if (set.base_assign != 10) {
                        status_codes st2 = replace_operations_in_decimal(expr, set, tree);
                        printf("[682] %s\n", tok);
                        if (st2 != OK) {
                            free(string1);
                            free(file_calculations);
                            free(expression1);
                            return st2;
                        }
                    }
                    status_codes st3 = calculate_expression(&result, expr, set, tree);
                    if (st3 != OK) { 
                        free(string1);                       
                        free(expression1);
                        fclose(file_calculations);
                        return st3;
                    }
                    if(var_name != NULL) {     
                        insert(tree, var_name, result);             
                    }
                }  
            }
        }   
        
        free(expression1);
        free(string1);
    }
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
            (*set).format = strdup(string);
            if ((*set).format == NULL) {
                fclose(file_settings);
                return NO_MEMORY;
            }

        } else if (strcmp(string, "--debug") == 0 || strcmp(string, "-d") == 0 || strcmp(string, "/debag") == 0) {
            (*set).isDebug = 1;
        } else {
            char string2[BUFSIZ];
            if(fscanf(file_settings, "%s", string2) == 1) {    
                status_codes st = add_synonym(table, string, string2);    
                if(st != OK) {
                    fclose(file_settings);
                    return st;
                }
            }
        }
    }
    

    fclose(file_settings);
    return OK;
}

int main(int argc, char** argv) {
//    if (argc < 2 || argc > 5) {
//        fprint_err(stdout, INVALID_ARGC);
//        return INVALID_ARGC;
//    }

    Format set;
    set.base_assign = 16;
    set.base_input = 10;
    set.base_output = 10;
//
//    if (argc >= 3) {
//        set.base_assign = atol(argv[2]);
//        if (argc >= 4) {
//            set.base_input = atol(argv[3]);
//            if (argc == 5) {
//                set.base_output = atol(argv[4]);
//            }
//        }
//    }

    SynonymTable* table = create_table();
    if (!table) {
        fprint_err(stdout, NO_MEMORY);
        return NO_MEMORY;
    }

    status_codes status = Settings("data1.txt", table, &set);//argv[1]

    if (status != OK) {
        free_table(table);
        fprint_err(stdout, status);
        return status;
    }

    TreeNode *root = getNode();
    if (root == NULL) {
        free_table(table);
        fprint_err(stdout, NO_MEMORY);
        return NO_MEMORY;
    }

    status_codes st = calculation("expression1.txt", set, root, *table);

    freeTree(root);
    free_table(table);
    free(set.format);

    if (st != OK) {
        fprint_err(stdout, st);
        return st;
    }

    return 0;
}