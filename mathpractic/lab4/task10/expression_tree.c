#include "stack.c"
#define MAX_SIZE 100

//объявление функций
status_codes checkBracketsBalance(char* expression);
bool isOperator(char operator);
status_codes infixToPostfix(char* infix, char** postfix);
int Priority(char operator);
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

bool isOperator(char operator) {
    return (operator == '-') || (operator == '+') || (operator == '*') || (operator == '%') || (operator == '/') || (operator == '^');
}

/*int Priority(char operator) {
    if (operator == 'not') return 1;
    else if (operator == 'add' || operator == 'sub'|| operator == 'xor') return 2;
    else if (operator == 'mult' || operator == 'rem' || operator == 'and') return 3;
    else if (operator == '^') return 3;
    return 0;
}*/
/*status_codes infixToPostfix(char* infix, char** postfix) {
    (*postfix) = (char*)malloc(strlen(infix) * 2 + 1);
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
        } else if (infix[i] == '(') {
            push(&operator_stack, &infix[i], sizeof(char));
        } else if (infix[i] == ')') {
            while (!isEmpty(&operator_stack) && *(char*)peek(&operator_stack) != '(') {
                postfix_expression[index++] = *(char*)pop(&operator_stack);
                postfix_expression[index++] = ' ';
            }
            pop(&operator_stack);
        } else if (isOperator(infix[i])) {
            while (!isEmpty(&operator_stack) && Priority(*(char*)peek(&operator_stack)) >= Priority(infix[i])) {
                postfix_expression[index++] = *(char*)pop(&operator_stack);
                postfix_expression[index++] = ' ';
            }
            push(&operator_stack, &infix[i], sizeof(char));
        } else if(isspace(infix[i])) {
            continue;
        } else {
            free(*postfix);
            free(postfix_expression);
            freeStack(&operator_stack);
            return WRONG_ELEMENT;
        }
    }

    while (!isEmpty(&operator_stack)) {
        postfix_expression[index++] = *(char*)pop(&operator_stack);
        postfix_expression[index++] = ' ';
    }

    postfix_expression[index] = '\0';
    printf("%s", postfix_expression);
    strcpy((*postfix), postfix_expression);
    free(postfix_expression);
    return OK;
}*/

void printfErr(FILE* file_err, char* file_name, status_codes error, int line) {
    fprintf(file_err, "Error in file %s on the expression number %d: ", file_name, line);
    fprint_err(file_err, error);
    fprintf(file_err, "\n");
}

status_codes prefixToPostfix(char *expression, char** postfix1) {
    char* postfix = (char*) malloc(sizeof (char) * (strlen(expression) + 1));
    if(postfix == NULL) {
        return NO_MEMORY;
    }

    char* token = strtok(expression, " ");
    char** tokens = (char**)malloc(sizeof (char*) * BUFSIZ);
    if(tokens == NULL) {
        free(postfix); // Free memory allocated for postfix
        return NO_MEMORY;
    }

    int index = 0;
    while(token != NULL) {
        tokens[index] = malloc(strlen(token) + 1);
        if(tokens[index] == NULL) {
            // Free memory allocated for tokens
            for (int i = 0; i < index; i++) {
                free(tokens[i]);
            }
            free(tokens);
            free(postfix);
            return NO_MEMORY;
        }
        strcpy(tokens[index], token);
        token = strtok(NULL, " ");
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

int calculationByPostfix(char *expression) {
    Stack stack;
    createStack(&stack);

    char *token = strtok(expression, " ");
    int result = 0; // Initialize result

    while (token != NULL) {
//        printf("%s\n", token);

        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            int tok = (int)atol(token);
            push(&stack, &tok, sizeof(int));
        } else {
            if (strcmp(token, "add") == 0 || strcmp(token, "sub") == 0 || strcmp(token, "mult") == 0 ||
                strcmp(token, "div") == 0 || strcmp(token, "pow") == 0 || strcmp(token, "rem") == 0 ||
                strcmp(token, "xor") == 0 || strcmp(token, "and") == 0 || strcmp(token, "or") == 0) {
                int operand1 = *(int *)pop(&stack);
                int operand2 = *(int *)pop(&stack);

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
                        printf("Division by zero\n");
                        exit(EXIT_FAILURE);
                    }
                } else if (strcmp(token, "pow") == 0) {
                    result = (int)pow(operand1, operand2);
                } else if (strcmp(token, "rem") == 0) {
                    result = operand1 % operand2;
                } else if (strcmp(token, "xor") == 0) {
                    result = operand1 ^ operand2;
                } else if (strcmp(token, "and") == 0) {
                    result = operand1 & operand2;
                } else if (strcmp(token, "or") == 0) {
                    result = operand1 | operand2;
                } else {
                    printf("Unknown operation: %s\n", token);
                    exit(EXIT_FAILURE);
                }
            } else if (strcmp(token, "not") == 0) {
                int operand = *(int *)pop(&stack);
                result = ~operand;
            }
            push(&stack, &result, sizeof(int));
        }
        token = strtok(NULL, " ");
    }

    return *(int *)pop(&stack);
}

int main() {

    char* postfix;
    char expression[] = "add mult 2 2 3";
    prefixToPostfix(expression, &postfix);
    int result = calculationByPostfix(postfix);
    printf("result: %d\n", result);

    char expression1[] = "3 2 1 mult add";
    int result1 = calculationByPostfix(expression1);
    printf("result: %d\n", result1);

//
//    char* postfix1;
//    char expression2[] = "23 add 4 mult 2";
//    infixToPostfix(expression2, &postfix1);
//
//    printf("%s", postfix1);
//
//    int result2 = calculationByPostfix(postfix1);
//
//    printf("result: %d", result2);
    return 0;
}
