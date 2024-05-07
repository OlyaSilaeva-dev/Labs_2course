#include <stdio.h>
#include <stdlib.h>

// Структура узла биномиальной кучи
typedef struct Node {
    int key;            // Значение ключа
    int priority;       // Приоритет
    int degree;         // Ранг (количество дочерних узлов)
    struct Node *parent;    // Родительский узел
    struct Node *child;     // Дочерний узел (любой)
    struct Node *sibling;   // Братский узел (следующий узел с тем же рангом)
} Node;

// Структура биномиальной кучи
typedef struct BinomialHeap {
    Node *head;         // Указатель на первый узел в списке корневых узлов
} BinomialHeap;

// Создание нового узла
Node* createNode(int key, int priority) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->key = key;
    newNode->priority = priority;
    newNode->degree = 0;
    newNode->parent = NULL;
    newNode->child = NULL;
    newNode->sibling = NULL;
    return newNode;
}

// Создание новой биномиальной кучи
BinomialHeap* createHeap() {
    BinomialHeap *newHeap = (BinomialHeap*)malloc(sizeof(BinomialHeap));
    if (newHeap == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newHeap->head = NULL;
    return newHeap;
}

// Слияние двух биномиальных куч
BinomialHeap* mergeHeaps(BinomialHeap *h1, BinomialHeap *h2) {
    BinomialHeap *newHeap = createHeap();
    Node *temp1 = h1->head;
    Node *temp2 = h2->head;
    Node *prev = NULL;
    Node *curr = NULL;
    
    while (temp1 != NULL || temp2 != NULL) {
        if (temp1 == NULL) {
            curr = temp2;
            temp2 = temp2->sibling;
        } else if (temp2 == NULL) {
            curr = temp1;
            temp1 = temp1->sibling;
        } else {
            if (temp1->degree <= temp2->degree) {
                curr = temp1;
                temp1 = temp1->sibling;
            } else {
                curr = temp2;
                temp2 = temp2->sibling;
            }
        }
        
        if (prev == NULL) {
            newHeap->head = curr;
        } else {
            prev->sibling = curr;
        }
        prev = curr;
    }
    
    return newHeap;
}

// Объединение двух биномиальных куч с разрушением исходных
BinomialHeap* unionHeaps(BinomialHeap *h1, BinomialHeap *h2) {
    BinomialHeap *mergedHeap = mergeHeaps(h1, h2);
    Node *prev = NULL;
    Node *curr = mergedHeap->head;
    Node *next = curr->sibling;
    
    while (next != NULL) {
        if ((curr->degree != next->degree) || 
            (next->sibling != NULL && next->sibling->degree == curr->degree)) {
            prev = curr;
            curr = next;
        } else {
            if (curr->priority >= next->priority) {
                if (prev == NULL) {
                    mergedHeap->head = next;
                } else {
                    prev->sibling = next;
                }
                curr->parent = next;
                curr->sibling = next->child;
                next->child = curr;
                next->degree++;
                curr = next;
            } else {
                if (prev == NULL) {
                    mergedHeap->head = curr;
                } else {
                    prev->sibling = curr;
                }
                next->parent = curr;
                next->sibling = curr->child;
                curr->child = next;
                curr->degree++;
            }
        }
        next = curr->sibling;
    }
    
    return mergedHeap;
}

// Добавление элемента в биномиальную кучу
BinomialHeap* insert(BinomialHeap *heap, int key, int priority) {
    BinomialHeap *newHeap = createHeap();
    Node *temp = createNode(key, priority);
    newHeap->head = temp;
    return unionHeaps(heap, newHeap);
}

// Нахождение узла с максимальным приоритетом
Node* findMax(BinomialHeap *heap) {
    if (heap->head == NULL)
        return NULL;
    
    Node *maxNode = heap->head;
    Node *temp = heap->head->sibling; // Start from the second node
    while (temp != NULL) {
        if (temp->priority > maxNode->priority)
            maxNode = temp;
        temp = temp->sibling;
    }
    
    return maxNode;
}

// Удаление узла с максимальным приоритетом
BinomialHeap* deleteMax(BinomialHeap *heap) {
    Node *maxNode = findMax(heap);
    if (maxNode == NULL)
        return heap;
    
    BinomialHeap *newHeap = createHeap();
    Node *temp = heap->head;
    Node *prev = NULL;
    
    while (temp != NULL) {
        if (temp != maxNode) {
            if (prev == NULL) {
                newHeap->head = temp;
            } else {
                prev->sibling = temp;
            }
            prev = temp;
        }
        temp = temp->sibling;
    }
    
    // Reversing the list of children of the max node
    Node *child = maxNode->child;
    Node *next = NULL;
    Node *prevChild = NULL;
    while (child != NULL) {
        next = child->sibling;
        child->sibling = prevChild;
        prevChild = child;
        child = next;
    }
    
    // Creating a new heap from the reversed list of children
    BinomialHeap *childHeap = createHeap();
    childHeap->head = prevChild;
    
    // Union of the new heap and the remaining heap
    return unionHeaps(newHeap, childHeap);
}

// Освобождение памяти, занимаемой узлом
void freeNode(Node *node) {
    if (node == NULL)
        return;
    
    // Рекурсивно освобождаем память, занимаемую дочерними узлами
    freeNode(node->child);
    // Освобождаем память для текущего узла
    free(node);
}

// Освобождение памяти, занимаемой биномиальной кучей
void freeHeap(BinomialHeap *heap) {
    if (heap == NULL)
        return;
    
    // Освобождаем память для каждого узла в куче
    freeNode(heap->head);
    // Освобождаем память для самой кучи
    free(heap);
}

void printHeap(Node *node) {
    if (node == NULL)
        return;

    // Печать текущего узла
    printf("Key: %d, Priority: %d\n", node->key, node->priority);

    // Рекурсивный вызов для дочерних узлов
    printHeap(node->child);
    
    // Рекурсивный вызов для братских узлов
    printHeap(node->sibling);
}


int main() {
    // Создание новой биномиальной кучи
    BinomialHeap *heap = createHeap();
    
    // Вставка элементов в кучу
    heap = insert(heap, 5, 10);
    heap = insert(heap, 3, 20);
    heap = insert(heap, 7, 5);
    heap = insert(heap, 9, 15);
    
    // Печать исходной кучи
    printf("Original Heap:\n");
    printHeap(heap);

    // Нахождение узла с максимальным приоритетом
    Node *maxNode = findMax(heap);
    if (maxNode != NULL)
        printf("Max priority node: key = %d, priority = %d\n", maxNode->key, maxNode->priority);
    
    // Удаление узла с максимальным приоритетом
    heap = deleteMax(heap);

    // Печать кучи после удаления максимального узла
    printf("\nHeap after deleting max node:\n");
    printHeap(heap);
    
    // Освобождение памяти, занимаемой кучей
    freeHeap(heap);
    
    return 0;
}
