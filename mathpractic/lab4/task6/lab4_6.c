#include "../header.h"
#define MAX_LENGTH 200
#define FILE_NAME_SIZE 10
#define ALPHABET_SIZE 62

struct TreeNode {
    char data;
    struct TreeNode* left;
    struct TreeNode* right;
};

struct Stack {
    int top;
    unsigned capacity;
    struct TreeNode** array;
};

struct Variable {
    char name;
    int value;
};

int isOperand(char ch);
int isOperator(char ch);
struct TreeNode* createNode(char data);
struct Stack* createStack(unsigned capacity);
void push(struct Stack* stack, struct TreeNode* item);
struct TreeNode* pop(struct Stack* stack);
struct TreeNode* peek(struct Stack* stack);
int isEmpty(struct Stack* stack);
int precedence(char op);
void freeTree(struct TreeNode* root);
struct TreeNode* buildExpressionTree(char* infixExpression);
int evaluateExpressionTree(struct TreeNode* root);
status_codes generateNameOutput(char** file_output);

status_codes main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stdout, INVALID_ARGC);
        return INVALID_ARGC;
    }

    FILE* file_input = fopen(argv[1], "r");
    if (file_input == NULL) {
        fprintf(stdout, FILE_NOT_OPEN);
        return FILE_NOT_OPEN;
    }
    char* infixExpression;
    infixExpression = (char*)malloc(MAX_LENGTH);
    if (infixExpression == NULL) {
        fclose(file_input);
        return NO_MEMORY;
    }

    fgets(infixExpression, MAX_LENGTH, file_input);
    fclose(file_input);

    // Подсчет количества уникальных переменных в выражении
    int numVariables = 0;
    struct Variable variables[52]; // Предполагаем, что переменные - одиночные буквы английского алфавита

    for (int i = 0; infixExpression[i] != '\0'; ++i) {
        if (isalpha(infixExpression[i])) {
            int j;
            for (j = 0; j < numVariables; ++j) {
                if (variables[j].name == infixExpression[i])
                    break;
            }
            if (j == numVariables) {
                variables[numVariables].name = infixExpression[i];
                variables[numVariables].value = 0;
                ++numVariables;
            }
        }
    }

    // Создание таблицы истинности
    int numCombinations = 1 << numVariables; // 2^numVariables
    int values[numVariables];

    char* output_name = (char*)malloc(sizeof(char) * FILE_NAME_SIZE);
    if (output_name == NULL || generateNameOutput(&output_name) != OK) {
        free(infixExpression);
        fprint_err(stdout, NO_MEMORY);
        return NO_MEMORY;
    }
    FILE* file_output;
    file_output = fopen(output_name, "w");
    free(output_name);

    fprintf(file_output, "\nThe Truth table:\n");
    fprintf(file_output, "-------------------\n");

    for (int i = 0; i < numVariables; ++i) {
        fprintf(file_output, "%c\t", variables[i].name);
    }
    fprintf(file_output, "Result\n");

    for (int i = 0; i < numCombinations; ++i) {
        for (int j = 0; j < numVariables; ++j) { // Заполнение значений переменных для текущей комбинации
            values[j] = (i >> (numVariables - 1 - j)) & 1;
        }

        for (int j = 0; j < numVariables; ++j) { // Вывод значений переменных
            fprintf(file_output, "%d\t", values[j]);
            variables[j].value = values[j];
        }

        char tmp[MAX_LENGTH];  // Вычисление и вывод результата выражения
        strcpy(tmp, infixExpression);
        size_t tmpLength = strlen(tmp);
        for (int k = 0; k < tmpLength; ++k) {
            if (isalpha(tmp[k])) {
                for (int l = 0; l < numVariables; ++l) {
                    if (variables[l].name == tmp[k]) {
                        tmp[k] = variables[l].value + '0';
                        break;
                    }
                }
            }
        }

        struct TreeNode* root = buildExpressionTree(tmp);

        int result = evaluateExpressionTree(root);

        fprintf(file_output, "%d\n", result);

        freeTree(root);
    }

    fclose(file_output);
    free(infixExpression);
    return OK;
}

status_codes generateNameOutput(char** file_output) {
    char* file_name = (char*)malloc(sizeof(char) * (FILE_NAME_SIZE + 1));
    if (file_name == NULL) {
        return NO_MEMORY;
    }

    char alphabet[ALPHABET_SIZE];
    int index = 0;
    for (int i = 0; i < 26; i++) {
        alphabet[index++] = 'a' + i;
    }

    for (int i = 0; i < 26; i++) {
        alphabet[index++] = 'A' + i;
    }

    for (int i = 0; i < 10; i++) {
        alphabet[index++] = '0' + i;
    }

    srand(time(NULL));
    for (int i = 0; i < FILE_NAME_SIZE; i++) {
        file_name[i] = alphabet[rand() % ALPHABET_SIZE];
    }

    file_name[FILE_NAME_SIZE] = '\0';

    strcat(file_name, ".txt\0");

    strcpy(*file_output, file_name);
    free(file_name);
    return OK;
}

struct TreeNode* createNode(char data) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    if(newNode == NULL) {
        return NULL;
    }
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    return newNode;
}

struct Stack* createStack(unsigned capacity) {
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    if (stack == NULL) {
        return NULL;
    }
    stack->top = -1;
    stack->capacity = capacity;
    stack->array = (struct TreeNode**)malloc(stack->capacity * sizeof(struct TreeNode*));
    return stack;
}

void push(struct Stack* stack, struct TreeNode* item) {
    stack->array[++stack->top] = item;
}

struct TreeNode* pop(struct Stack* stack) {
    if (stack->top == -1)
        return NULL;
    return stack->array[stack->top--];
}

struct TreeNode* peek(struct Stack* stack) {
    if (stack->top == -1)
        return NULL;
    return stack->array[stack->top];
}

int isOperand(char ch) {
    return isalnum(ch);
}

int isOperator(char ch) {
    switch (ch) {
        case '&':
        case '|':
        case '~':
        case '-':
        case '+':
        case '<':
        case '=':
        case '!':
        case '?':
            return 1;
        default:
            return 0;
    }
}

int isParenthesis(char ch) {
    return (ch == '(' || ch == ')');
}

int isEmpty(struct Stack* stack) {
    return stack->top == -1;
}

int precedence(char op) {
    switch (op) {
        case '|':
            return 1;
        case '-':
        case '+':
        case '<':
        case '=':
            return 2;
        case '~':
            return 3;
        case '&':
            return 4;
        case '!':
        case '?':
            return 5;
        default:
            return 0;
    }
}

void freeTree(struct TreeNode* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

status_codes addNode(struct Stack* operators, struct Stack* operands) {
    struct TreeNode* operatorNode = createNode(pop(operators)->data);
    if (operatorNode == NULL) {
        return NO_MEMORY;
    }
    operatorNode->right = pop(operands);
    operatorNode->left = pop(operands);
    push(operands, operatorNode);
    return OK;
}

void addNodeFor2Operators(struct Stack* operators, struct Stack* operands, char operator) {
    struct TreeNode* operatorNode = createNode(operator);
    if(operatorNode == NULL) {
        return;
    }
    while (!isEmpty(operators) && precedence(operatorNode->data) <= precedence(peek(operators)->data)) {
        struct TreeNode* temp = createNode(pop(operators)->data);
        temp->right = pop(operands);
        temp->left = pop(operands);
        push(operands, temp);
    }
    push(operators, operatorNode);
}

struct TreeNode* buildExpressionTree(char* infixExpression) {
    struct Stack* operators = createStack(strlen(infixExpression));
    struct Stack* operands = createStack(strlen(infixExpression));
    if(operators == NULL || operands == NULL) {
        return NULL;
    }

    for (int i = 0; infixExpression[i] != '\0'; ++i) {
        if (infixExpression[i] == ' ')
            continue;

        if (isOperand(infixExpression[i])) {
            struct TreeNode* operandNode = createNode(infixExpression[i]);
            push(operands, operandNode);
        } else if (isOperator(infixExpression[i])) {
            while (!isEmpty(operators) && precedence(infixExpression[i]) <= precedence(peek(operators)->data)) {
                addNode(operators, operands);
            }
            push(operators, createNode(infixExpression[i]));
        } else if (isParenthesis(infixExpression[i])) {
            if (infixExpression[i] == '(') {
                struct TreeNode* parenthesisNode = createNode(infixExpression[i]);
                push(operators, parenthesisNode);
            } else if (infixExpression[i] == ')') {
                while (!isEmpty(operators) && peek(operators)->data != '(') {
                    addNode(operators, operands);
                }
                pop(operators);
            }
        } else if (infixExpression[i] == '>') {
            if (infixExpression[i + 1] == '-') {
            }
        } else if (infixExpression[i] == '+') {
            if (infixExpression[i + 1] == '>') {
                i++;
                addNodeFor2Operators(operators, operands, '<');
            }
        } else if (infixExpression[i] == '<') {
            if (infixExpression[i + 1] == '>') {
                i++;
                addNodeFor2Operators(operators, operands, '<');
            }
        } else if (infixExpression[i] == '=') {
            addNodeFor2Operators(operators, operands, '=');
        } else if (infixExpression[i] == '!') {
            if (infixExpression[i + 1] == '-') {
                i++;
                addNodeFor2Operators(operators, operands, '!');
            }
        } else if (infixExpression[i] == '?') {
            if (infixExpression[i + 1] == '-') {
                i++;
                addNodeFor2Operators(operators, operands, '?');
            }
        }
    }

    while (!isEmpty(operators)) {
        addNode(operators, operands);
    }

    return pop(operands);
}

int evaluateExpressionTree(struct TreeNode* root) {
    if (root == NULL)
        return 0;

    if (isOperand(root->data))
        return root->data - '0';

    int leftValue = evaluateExpressionTree(root->left);
    int rightValue = evaluateExpressionTree(root->right);

    switch (root->data) {
        case '&':
            return leftValue && rightValue;
        case '|':
            return leftValue || rightValue;
        case '~':
            return !rightValue;
        case '-':
            return !leftValue || rightValue;
        case '+':
            return !(leftValue && rightValue);
        case '<':
            return (leftValue + rightValue) % 2;
        case '=':
            return leftValue == rightValue;
        case '!':
            return leftValue && rightValue;
        case '?':
            return leftValue || !rightValue;
        default:
            return 0;
    }
}
