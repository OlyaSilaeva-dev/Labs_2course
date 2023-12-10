#include "stack.c"

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

int Priority(char operator) {
    if (operator == '+' || operator == '-') return 1;
    else if (operator == '*' || operator == '/' || operator == '%') return 2;
    else if (operator == '^') return 3;
    return 0;
}

status_codes infixToPostfix(char* infix, char** postfix) {
    (*postfix) = (char*)malloc(strlen(infix) + 1);
    if (*postfix == NULL) {
        return NO_MEMORY;
    }
    Stack operator_stack;
    createStack(&operator_stack);

    char* postfix_expression = (char*)malloc((strlen(infix)) * 2);
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
            --i;  // Вернуться на 1 позицию, так как i уже инкрементирован в конце цикла
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
    strcpy((*postfix), postfix_expression);
    free(postfix_expression);
    return OK;
}

status_codes calculationByPostfix(char* expression, int* result) {
    Stack operands;
    createStack(&operands);

    for (int i = 0; expression[i] != '\0'; ++i) {
        if (isdigit(expression[i])) {
            int operand = 0;
            while (isdigit(expression[i])) {
                operand = operand * 10 + (expression[i] - '0');
                ++i;
            }
            push(&operands, &operand, sizeof(int));
            --i;
        } else if (isspace(expression[i])) {
            continue;
        } else {
            int operand2 = *(int*)pop(&operands);
            int operand1 = *(int*)pop(&operands);
            int res;

            switch (expression[i]) {
                case '+':
                    res = operand1 + operand2;
                    break;
                case '-':
                    res = operand1 - operand2;
                    break;
                case '*':
                    res = operand1 * operand2;
                    break;
                case '/':
                    if (operand2 != 0) {
                        res = operand1 / operand2;
                    } else {
                        freeStack(&operands);
                        return DIVISION_BY_ZERO;
                    }
                    break;

                default:
                    freeStack(&operands);
                    return WRONG_ELEMENT;
                    break;
            }

            push(&operands, &res, sizeof(int));
        }
    }

    if (isEmpty(&operands)) {
        freeStack(&operands);
        return EMPTY_OPERAND_STACK;
    }

    *result = *(int*)pop(&operands);
    freeStack(&operands);
    return OK;
}

void printfErr(FILE* file_err, char* file_name, status_codes error, int line) {
    fprintf(file_err, "Error in file %s on the expression number %d: ", file_name, line);
    fprint_err(file_err, error);
    fprintf(file_err, "\n");
}

int main(int argc, char** argv) {
    FILE* file_err = fopen("err.txt", "w");
    if (file_err == NULL) {
        fprint_err(stdout, FILE_NOT_OPEN);
        return FILE_NOT_OPEN;
    }

    for (int i = 1; i < argc; i++) {  
        FILE* file_input = fopen(argv[i], "r");
        if (file_input == NULL) {
            continue;
        }

        char expression[BUFSIZ];
        while (fgets(expression, sizeof(expression), file_input) != NULL) {
            size_t length = strlen(expression);
            if (length > 0 && expression[length - 1] == '\n') {
                expression[length - 1] = '\0';
            }
            printf("%s\n", expression);

            status_codes st = checkBracketsBalance(expression);
            if (st != OK) {
                printfErr(file_err, argv[i], st, (i + 1));
                break;
            }

            char* postfixExpression;
            st = infixToPostfix(expression, &postfixExpression);
            if (st == OK) {
                printf("%s\n", postfixExpression);
            } else {
                printfErr(file_err, argv[i], st, (i + 1));
                break;
            }

            int result;
            st = calculationByPostfix(postfixExpression, &result);

            if(st == OK) {
                printf("%d\n", result);
            } else {
                printfErr(file_err, argv[i], st, (i + 1));
                free(postfixExpression); 
                break; 
            }
            free(postfixExpression);
        }
        fclose(file_input);
    }
    fclose(file_err);

    return OK;
}
