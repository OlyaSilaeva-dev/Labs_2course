#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

int main() {
    char infixExpression[100];
    
    printf("Enter a Boolean expression: ");
    fgets(infixExpression, sizeof(infixExpression), stdin);
    
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
    
    printf("\nThe Truth table:\n");
    printf("-------------------\n");
    
    // Вывод заголовка таблицы
    for (int i = 0; i < numVariables; ++i) {
        printf("%c\t", variables[i].name);
    }
    printf("Result\n");
    
     // Вывод значений переменных и результатов выражения
    for (int i = 0; i < numCombinations; ++i) {
        for (int j = 0; j < numVariables; ++j) { // Заполнение значений переменных для текущей комбинации
            values[j] = (i >> (numVariables - 1 - j)) & 1;
        }
        
       
        for (int j = 0; j < numVariables; ++j) { // Вывод значений переменных
            printf("%d\t", values[j]);
            variables[j].value = values[j];
        }
        
        char tmp[strlen(infixExpression)];  // Вычисление и вывод результата выражения
        strcpy(tmp, infixExpression);
        for (int k = 0; k < strlen(tmp); ++k) {
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
        printf("%d\n", result);

        freeTree(root);
    }
    
    return 0;
}

struct TreeNode* createNode(char data) {
    struct TreeNode* newNode = (struct TreeNode*)malloc(sizeof(struct TreeNode));
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    return newNode;
}

struct Stack* createStack(unsigned capacity) {
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
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

void addNode(struct Stack* operators, struct Stack* operands) {
    struct TreeNode* operatorNode = createNode(pop(operators)->data);
    operatorNode->right = pop(operands);
    operatorNode->left = pop(operands);
    push(operands, operatorNode);
}

void addNodeFor2Operators(struct Stack* operators, struct Stack* operands,char operator) {
    struct TreeNode* operatorNode = createNode(operator);
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
            // Handle the implication operator
            if (infixExpression[i + 1] == '-') {
            }
        } else if (infixExpression[i] == '+') {
            // Handle the co-implication operator
            if (infixExpression[i + 1] == '>') {
                i++; // Skip the next character
                struct TreeNode* operatorNode = createNode('+');
                while (!isEmpty(operators) && precedence(operatorNode->data) <= precedence(peek(operators)->data)) {
                    struct TreeNode* temp = createNode(pop(operators)->data);
                    temp->right = pop(operands);
                    temp->left = pop(operands);
                    push(operands, temp);
                }
                push(operators, operatorNode);
            }
        } else if (infixExpression[i] == '<') {
            if (infixExpression[i + 1] == '>') {
                i++;
                addNodeFor2Operators(operators, operands, '<');
            }
        } else if (infixExpression[i] == '=') {
            addNodeFor2Operators(operators, operands, '=');

        } else if (infixExpression[i] == '!') {
            // Handle the Sheffer stroke operator
            if (infixExpression[i + 1] == '-') {
                i++; // Skip the next character
                addNodeFor2Operators(operators, operands, '!');
            }
        } else if (infixExpression[i] == '?') {
            // Handle the logical function of Webb operator
            if (infixExpression[i + 1] == '-') {
                i++; // Skip the next character
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
