#include "../header.h"

typedef struct {
    char* name;
    int value;
} MemoryCell;

status_codes printValue(MemoryCell* memory, int size, const char* name);
status_codes initializeVariable(MemoryCell** memory, int* size, const char* name, int value);
status_codes evaluateExpression(MemoryCell* memory, int size, const char* expression, int* res);
void freeMemory(MemoryCell** memory, int size);
int compare(const void *a, const void *b);
int findVariable(MemoryCell* memory, int size, const char* name);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprint_err(stdout, INVALID_ARGC);
        return INVALID_ARGC;
    }

    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        fprint_err(stdout, FILE_NOT_OPEN);
        return FILE_NOT_OPEN;
    }

    MemoryCell* memory = NULL;
    int memorySize = 0;

    char line[BUFSIZ];
    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, " =;\n");

        if (token != NULL) {
            if (strcmp(token, "print") == 0) {
                token = strtok(NULL, ";\n");
                printValue(memory, memorySize, token);
            } else {
                char* name = token;
                token = strtok(NULL, ";\n"); // Пропускаем "="

                if (token != NULL) {
                    int value;
                    status_codes st = evaluateExpression(memory, memorySize, token, &value);
                    if(st != OK) {
                        fclose(file);
                        fprint_err(stdout, st);
                        return st;
                    }
                    initializeVariable(&memory, &memorySize, name, value);
                } else {
                    fclose(file);
                    fprint_err(stdout, INVALID_FORMAT);
                    return INVALID_FORMAT;
                }
            }
        }
    }

    fclose(file);
    freeMemory(&memory, memorySize);

    return OK;
}

// Сравнение для функции qsort
int compare(const void *a, const void *b) {
    return strcmp(((MemoryCell*)a)->name, ((MemoryCell*)b)->name);
}
int findVariable(MemoryCell* memory, int size, const char* name) {
    int left = 0;
    int right = size - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(memory[mid].name, name);

        if (cmp == 0) {
            return mid; 
        } else if (cmp < 0) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1; 
}

// Инициализация или обновление значения переменной
status_codes initializeVariable(MemoryCell** memory, int* size, const char* name, int value) {
    int index = findVariable(*memory, *size, name);

    if (index != -1) {
        (*memory)[index].value = value;
    } else {
        (*size)++;
        *memory = realloc(*memory, (*size) * sizeof(MemoryCell));
        if (memory == NULL) {
            return NO_MEMORY;
        }
        (*memory)[(*size) - 1].name = strdup(name);
        (*memory)[(*size) - 1].value = value;

        qsort(*memory, *size, sizeof(MemoryCell), compare);
    }
    return OK;
}

status_codes printValue(MemoryCell* memory, int size, const char* name) {
    if (name == NULL) {
        for (int i = 0; i < size; i++) {
            printf("%s=%d\n", memory[i].name, memory[i].value);
        }
    } else {
        int index = findVariable(memory, size, name);

        if (index != -1) {
            printf("%s=%d\n", name, memory[index].value);
        } else {
            return WRONG_ELEMENT;
        }
    }
    return OK;
}

// Вычисление значения выражения
status_codes evaluateExpression(MemoryCell* memory, int size, const char* expression, int* res) {
    int result = 0;
    int sign = 1; 

    while (*expression && (*expression != ';')) {
        if (isdigit(*expression)) {
            int number = 0;
            while (isdigit(*expression)) {
                number = number * 10 + (*expression - '0');
                expression++;
            }
            result += sign * number;

        } else if (*expression == '-') {
            sign = -1;
            expression++;

        } else if (*expression == '+') {
            sign = 1;
            expression++;

        } else if (*expression == '*') {
            expression++;
            int operand;
            status_codes st = evaluateExpression(memory, size, expression, &operand);
            if(st != OK) return st;

            result *= operand;

            while (*expression && (*expression != ';' && *expression != '+' && *expression != '-' && *expression != '*' && *expression != '/' && *expression != '%')) {
                expression++;
            }
        } else if (*expression == '/') {
            expression++;
            int operand; 
            status_codes st = evaluateExpression(memory, size, expression, &operand);
            if(st != OK)  return st;
            if (operand != 0) {
                result /= operand;
            } else {
                return DIVISION_BY_ZERO;
            }

            while (*expression && (*expression != ';' && *expression != '+' && *expression != '-' && *expression != '*' && *expression != '/' && *expression != '%')) {
                expression++;
            }
        } else if (*expression == '%') {
            expression++;
            int operand; 
            status_codes st = evaluateExpression(memory, size, expression, &operand);
            if(st != OK)  return st;

            if (operand != 0) {
                result %= operand;
            } else {
                return DIVISION_BY_ZERO;
            }

            while (*expression && (*expression != ';' && *expression != '+' && *expression != '-' && *expression != '*' && *expression != '/' && *expression != '%')) {
                expression++;
            }
        } else if (isalpha(*expression)) { //предполагаем, что буква - переменная
            char* variableName = (char*)malloc(sizeof(char) * BUFSIZ);
            int i = 0;
            
            while (isalpha(*expression) || isdigit(*expression) || *expression == '_') {
                variableName[i++] = *expression;
                expression++;
            }            
            variableName[i] = '\0';
            int index = findVariable(memory, size, variableName);
            
            if (index != -1) {
                result += sign * memory[index].value;
            } else {
                return WRONG_ELEMENT;
            }
        } else {
            expression++;
        }
    }

    (*res) = result;
    return OK;
}

void freeMemory(MemoryCell** memory, int size) {
    for (int i = 0; i < size; i++) {
        free((*memory)[i].name);
    }
    free(*memory);
}

