#include "stack.c"
#include "bor.c"
#define MAX_SIZE 100
#define MAX_LENGTH 100
//объявление функций
status_codes checkBracketsBalance(char* expression);
bool isOperator(char* operator, int index);
status_codes infixToPostfix(char* infix, char** postfix, TreeNode* variableTrie);
int Priority(char* operator);
void printfErr(FILE* file_err, char* file_name, status_codes error, int line);
//

status_codes checkBracketsBalance(char* expression) {
    Stack brackets_stack;
    createStack(&brackets_stack);

    int i = 0;
    while (expression[i] != '\0') {
        if (expression[i] == '(') {
            push(&brackets_stack, &expression[i], sizeof(char));
        } else if (expression[i] == ')') {
            if (isEmpty(&brackets_stack) || *(char*)pop(&brackets_stack) != '(') {
                freeStack(&brackets_stack);
                return BRACKETS_NOT_BALANCED;
            }
        }
        ++i;
    }

    bool isBalanced = isEmpty(&brackets_stack);
    freeStack(&brackets_stack);
    if (!isBalanced) {
        return BRACKETS_NOT_BALANCED;
    }
    return OK;
}

bool isOperator(char* string, int index) {
    char* operators[] = {"add", "sub", "mult", "div", "pow", "rem", "xor", "and", "or", "not"};
    int numOperators = sizeof(operators) / sizeof(operators[0]);

    for (int i = 0; i < numOperators; ++i) {
        if (strcmp(string + index, operators[i]) == 0) {
            return true;
        }
    }
    return false;
}

int Priority(char* operator) {
    if (strcmp(operator, "add") == 0 || strcmp(operator, "sub") == 0) {
        return 1;
    } else if (strcmp(operator, "mult") == 0 || strcmp(operator, "div") == 0 || strcmp(operator, "rem") == 0) {
        return 2;
    } else if (strcmp(operator, "xor") == 0 || strcmp(operator, "and") == 0 || strcmp(operator, "or") == 0) {
        return 3;
    } else if (strcmp(operator, "pow") == 0) {
        return 4;
    } else if (strcmp(operator, "not") == 0) {
        return 5;
    }
    return 0;
}

void copyOperator(Stack *operator_stack, char** postfix_expression, int* index ){
    char* op = (char*)pop(&(*operator_stack));
    for (int j = 0; op[j] != '\0'; j++) {
        (*postfix_expression)[(*index)++] = op[j];
    }
    (*postfix_expression)[(*index)++] = ' ';
    free(op);
}


status_codes infixToPostfix(char* infix, char** postfix, TreeNode *variableTrie) {
    *postfix = (char*)malloc(strlen(infix) * 2 + 1);
    if (*postfix == NULL) {
        return NO_MEMORY;
    }

    Stack operator_stack;
    createStack(&operator_stack);

    char* postfix_expression = (char*)malloc((strlen(infix)) * 2 + 1);
    if (postfix_expression == NULL) {
        free(*postfix);
        return NO_MEMORY;
    }
    int index = 0;

    for (int i = 0; infix[i] != '\0'; ++i) {
        if (isdigit(infix[i])) {
            while (isdigit(infix[i])) {
                postfix_expression[index++] = infix[i++];
            }
            postfix_expression[index++] = ' ';
            --i;
        } else if (isalpha(infix[i]) || infix[i] == '_') {
            int start = i;
            while (isalpha(infix[i]) || isdigit(infix[i]) || infix[i] == '_') {
                ++i;
            }
            int len = i - start;
            char* variable = (char*)malloc(len + 1);
            if (variable == NULL) {
                free(*postfix);
                free(postfix_expression);
                freeStack(&operator_stack);
                return NO_MEMORY;
            }
            strncpy(variable, infix + start, len);
            variable[len] = '\0';
            --i;

            TreeNode* node = search(variableTrie, variable);
            if (node == NULL) {
                if (!isOperator(variable, 0)) {
                    free(variable);
                    free(*postfix);
                    free(postfix_expression);
                    freeStack(&operator_stack);
                    return WRONG_ELEMENT;
                }
                while (!isEmpty(&operator_stack) && Priority((char*)peek(&operator_stack)) >= Priority(variable)) {
                    copyOperator(&operator_stack, &postfix_expression, &index);
                }
                push(&operator_stack, variable, strlen(variable) + 1);
            } else {                
                uint32_t value = node->value;
                char valueStr[12]; // Максимальная длина для int
                sprintf(valueStr, "%u", value);
                for (int j = 0; valueStr[j] != '\0'; ++j) {
                    postfix_expression[index++] = valueStr[j];
                }
                postfix_expression[index++] = ' '; 
            }
            free(variable);
        } else if (infix[i] == '(') {
            push(&operator_stack, &infix[i], sizeof(char));
        } else if (infix[i] == ')') {
            while (!isEmpty(&operator_stack) && *(char*)peek(&operator_stack) != '(') {
                copyOperator(&operator_stack, &postfix_expression, &index);
            }
            free(pop(&operator_stack));
        } else if (!isspace(infix[i])) {
            int start = i;
            while (!isspace(infix[i]) && infix[i] != '\0' && infix[i] != '(' && infix[i] != ')') {
                ++i;
            }
            int opLen = i - start;
            char* operator = (char*)malloc(opLen + 1);
            if (operator == NULL) {
                free(*postfix);
                free(postfix_expression);
                freeStack(&operator_stack);
                return NO_MEMORY;
            }
            strncpy(operator, infix + start, opLen);
            operator[opLen] = '\0';
            --i;

            if (!isOperator(operator, 0)) {
                free(operator);
                free(*postfix);
                free(postfix_expression);
                freeStack(&operator_stack);
                return WRONG_ELEMENT;
            }

            while (!isEmpty(&operator_stack) && Priority((char*)peek(&operator_stack)) >= Priority(operator)) {
                copyOperator(&operator_stack, &postfix_expression, &index);
            }
            push(&operator_stack, operator, strlen(operator) + 1);
            free(operator);
        }
    }

    while (!isEmpty(&operator_stack)) {
        copyOperator(&operator_stack, &postfix_expression, &index);
    }

    postfix_expression[index] = '\0';
    strcpy(*postfix, postfix_expression);
    freeStack(&operator_stack);
    free(postfix_expression);
    return OK;
}

status_codes prefixToPostfix(char *expression, char** postfix1) {
    char* postfix = (char*) malloc(sizeof (char) * (strlen(expression) + 1));
    if(postfix == NULL) {
        return NO_MEMORY;
    }

    char* token = strtok(expression, " (),");
    char** tokens = (char**)malloc(sizeof (char*) * BUFSIZ);
    if(tokens == NULL) {
        free(postfix); 
        return NO_MEMORY;
    }

    int index = 0;
    while(token != NULL) {
        tokens[index] = malloc(strlen(token) + 1);
        if(tokens[index] == NULL) {
            for (int i = 0; i < index; i++) {
                free(tokens[i]);
            }
            free(tokens);
            free(postfix);
            return NO_MEMORY;
        }
        strcpy(tokens[index], token);
        token = strtok(NULL, " (),");
        index++;
    }

    postfix[0] = '\0';

    for(int i = index - 1; i >= 0 ; i--) {
        strcat(postfix, tokens[i]);
        strcat(postfix, " ");
    }

    *postfix1 = postfix;

    // Free memory allocated for tokens
    for (int i = 0; i < index; i++) {
        free(tokens[i]);
    }
    free(tokens);
    return OK;
}
uint32_t st(uint32_t base, uint32_t exp) {
    uint32_t result = 1;
    while (exp > 0) {
        if (exp & 1) { 
            result = ((uint64_t)result * base) % UINT32_MAX;
        }
        exp >>= 1; 
        base = ((uint64_t)base * base) % UINT32_MAX; 
    }
    return result;
}

uint32_t calculationByPostfix(char *expression, TreeNode *variableTrie) {
    Stack stack;
    createStack(&stack);
    char *token = strtok(expression, " ,()\0");

    uint32_t result = 0; // Initialize result

    while (token != NULL) {
        if (isdigit(token[0])) {
            uint32_t tok = (uint32_t)atol(token);
            push(&stack, &tok, sizeof(uint32_t));
        } else if (isOperator(token, 0)) {
            if (strcmp(token, "not") == 0) {
                uint32_t* operandPtr = (uint32_t *)pop(&stack);
                uint32_t operand = *operandPtr;
                free(operandPtr);
                result = ~operand;
                push(&stack, &result, sizeof(uint32_t));
            } else {
                uint32_t* operand2Ptr = (uint32_t *)pop(&stack);
                uint32_t operand2 = *operand2Ptr;
                free(operand2Ptr);

                uint32_t* operand1Ptr = (uint32_t *)pop(&stack);
                uint32_t operand1 = *operand1Ptr;
                free(operand1Ptr);

                if (strcmp(token, "add") == 0) {
                    result = operand1 + operand2;
                } else if (strcmp(token, "sub") == 0) {
                    result = operand1 - operand2;
                } else if (strcmp(token, "mult") == 0) {
                    result = operand1 * operand2;
                } else if (strcmp(token, "div") == 0) {
                    if (operand2 != 0) {
                        result = operand1 / operand2;
                    } else {
                        freeStack(&stack);
                        return DIVISION_BY_ZERO;
                    }
                } else if (strcmp(token, "pow") == 0) {
                    result = st(operand1, operand2);
                } else if (strcmp(token, "rem") == 0) {
                    result = operand1 % operand2;
                } else if (strcmp(token, "xor") == 0) {
                    result = operand1 ^ operand2;
                } else if (strcmp(token, "and") == 0) {
                    result = operand1 & operand2;
                } else if (strcmp(token, "or") == 0) {
                    result = operand1 | operand2;
                }
                push(&stack, &result, sizeof(uint32_t));
            }
        } else {
            TreeNode* node = search(variableTrie, token);
            if (node != NULL) {
                uint32_t value = node->value;
                push(&stack, &value, sizeof(uint32_t));
            } else {
                freeStack(&stack);
                return WRONG_ELEMENT;
            }
        }
        token = strtok(NULL, " ,()");
    }

    uint32_t* resPtr = (uint32_t *)pop(&stack);
    uint32_t res = *resPtr;
    free(resPtr);
    return res;
}

// int main() {

//     TreeNode *variableTrie = getNode();
//     insert(variableTrie, "x_1", 2);
//     insert(variableTrie, "y_1", 5);
//     insert(variableTrie, "z_1", 4);

    // char* postfix;
    // char expression[] = "add(mult(z,2),3)";
    // prefixToPostfix(expression, &postfix);

    // printf("%s", postfix);
    // int result = calculationByPostfix(postfix, variableTrie);
    // printf("result: %d\n", result);
    // free(postfix);

    // char expression1[] = "(3,(2,x)mult)add";
    // int result1 = calculationByPostfix(expression1, variableTrie);
    // printf("result: %d\n", result1);
    // isOperator("div ", 1);

//     char* postfix1;
//     char expression2[] = "x_1 add y_1 mult z_1";
//     infixToPostfix(expression2, &postfix1, variableTrie);

//     printf("%s\n", postfix1);

//     uint32_t result2 = calculationByPostfix(postfix1, variableTrie);
//     free(postfix1);
//     printf("result: %d", result2);
//     return 0;
// }
