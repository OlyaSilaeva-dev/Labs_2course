#include "../header.h"

typedef struct Node {
    void* data;
    struct Node* next;
} Node;

typedef struct Stack {
    Node* top;
} Stack;

bool isEmpty(Stack* stack) {
    return stack->top == NULL;
}

void createStack(Stack* stack) {
    stack->top = NULL;
}

status_codes push(Stack* stack, void* value, size_t data_size) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        return NO_MEMORY;
    }
    newNode->data = malloc(data_size);
    if (newNode->data == NULL) {
        free(newNode);
        return NO_MEMORY;
    }
    memcpy(newNode->data, value, data_size);
    newNode->next = stack->top;
    stack->top = newNode;
    return OK;
}

void* pop(Stack* stack) {
    if (isEmpty(stack)) {
        return NULL;
    }
    Node* top = stack->top;
    void* data = top->data;
    stack->top = top->next;
    free(top);      // освобождение памяти для узла
    return data;
}


void* peek(Stack* stack) {
    if (isEmpty(stack)) {
        return NULL;
    }
    return stack->top->data;
}

void freeStack(Stack* stack) {
    while (!isEmpty(stack)) {
        free(pop(stack));
    }
}

void printStack(Stack* stack, void (*printFunction)(void*)) {
    if (isEmpty(stack)) {
        printf("Stack is empty.\n");
        return;
    }

    printf("Stack contents:\n");
    Node* current = stack->top;
    while (current != NULL) {
        printFunction(current->data);
        current = current->next;
    }
}

void printInt(void* data) {
    printf("%d\n", *((int*)data));
}


// int main() {
//     int a[3] = {1, 2, 3};
//     Stack stack;
//     createStack(&stack);
    
//     for (int i = 0; i < 3; i++) {
//         push(&stack, &a[i], sizeof(int));
//     }
    
//     while (stack.top != NULL) {
//         int* value = (int*)pop(&stack);
//         printf("%d\n", *value);
//         free(value);
//     }
    
//     freeStack(&stack);
//     return 0;
// }