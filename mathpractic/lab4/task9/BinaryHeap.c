#include <stdio.h>
#include <stdlib.h>

typedef struct BinaryHeap {
    int* heap;
    int size;
} BinaryHeap;

void createBinaryHeap(BinaryHeap* pq){
    pq->heap = NULL;
    pq->size = 0;
}

void swap(int *a, int *b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void heapyfy_up(BinaryHeap *pq, int index){ //просеивание вверх указанного элемента в куче
    while (index > 0 && pq->heap[index] > pq->heap[(index - 1) / 2]){
        swap(&pq->heap[index], &pq->heap[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}

void heapify_down(BinaryHeap *pq, int index){ //просеивание вниз элемента в куче
    int left_child, right_child, largest;
    while (2*index + 1 < pq->size) {
        left_child = 2*index + 1;
        right_child = 2*index + 2;
        largest = left_child;
        if (right_child < pq->size && pq->heap[right_child] > pq->heap[left_child]) {
            largest = right_child;
        }
        if (pq->heap[index] >= pq->heap[largest]) {
            break;
        }
        swap(&pq->heap[index], &pq->heap[largest]);
        index = largest;
    }
}

void insert(BinaryHeap *pq, int item){ //добавление по ключу
    int* tmp = (int*)realloc(pq->heap, (pq->size + 1) * sizeof(int));
    if (tmp == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    pq->heap = tmp;
    pq->heap[pq->size++] = item;
    heapyfy_up(pq, pq->size - 1);
}

int extract_max(BinaryHeap *pq) { //возвращает элемент с максимальным приоритетом, заменяет корневой элемент последним элементом кучи
    if (pq->size == 0) {
        printf("Priority queue underflow\n");
        return -1;
    }
    int max_item = pq->heap[0];
    pq->heap[0] = pq->heap[--pq->size];
    pq->heap = (int*)realloc(pq->heap, pq->size * sizeof(int));
    if (pq->heap == NULL && pq->size > 0) {
        printf("Memory allocation failed\n");
        return -1;
    }
    heapify_down(pq, 0);
    return max_item;
}

int peek_max(BinaryHeap *pq) { //возвращает элемент с макс. приоритетом не удаляя его из очереди
    if(pq->size == 0) {
        printf("Priority queue is empty\n");
        return -1;
    }
    return pq->heap[0];
}

//слияние без разрушения
void merge_queues(const BinaryHeap *pq1, const BinaryHeap *pq2, BinaryHeap *result) {
    result->size = 0;    
    // Merge elements from pq1 and pq2 into result queue
    for (int i = 0; i < pq1->size; ++i) {
        insert(result, pq1->heap[i]);
    }
    for (int i = 0; i < pq2->size; ++i) {
        insert(result, pq2->heap[i]);
    }
}

//слияние с разрушением
void merge_and_destroy_queues(BinaryHeap *pq1, BinaryHeap *pq2, BinaryHeap *result) {
    result->size = 0;
    
    // Merge elements from pq1 and pq2 into result queue
    for (int i = 0; i < pq1->size; ++i) {
        insert(result, pq1->heap[i]);
    }
    for (int i = 0; i < pq2->size; ++i) {
        insert(result, pq2->heap[i]);
    }
    
    // Reset the sizes of pq1 and pq2 to indicate that their content is no longer valid
    pq1->size = 0;
    pq2->size = 0;
}

// int main() {
//     BinaryHeap p1;
//     BinaryHeap p2;
//     createBinaryHeap(&p1);
//     createBinaryHeap(&p2);

//     insert(&p1, 1);
//     insert(&p1, 3);
//     insert(&p1, 2);

//     insert(&p2, 4);
//     insert(&p2, 5);
//     insert(&p2, 0);
    
//     // Пример использования функций слияния
//     BinaryHeap merged;
//     merged.size = 0;
//     merged.heap = NULL;

//     for (int i = 0; i < p1.size; ++i) {
//         printf("%d ", p1.heap[i]);
//     }
//     printf("\n");

//     for (int i = 0; i < p2.size; ++i) {
//         printf("%d ", p2.heap[i]);
//     }
    
//     printf("\n");

//     merge_queues(&p1, &p2, &merged); // Слияние без разрушения
//     printf("Merged without destruction: ");
//     for (int i = 0; i < merged.size; ++i) {
//         printf("%d ", merged.heap[i]);
//     }
//     printf("\n");

//     merge_and_destroy_queues(&p1, &p2, &merged); // Слияние с разрушением
//     printf("Merged with destruction: ");
//     for (int i = 0; i < merged.size; ++i) {
//         printf("%d ", merged.heap[i]);
//     }
//     printf("\n");

//     free(p1.heap);
//     free(p2.heap);
//     free(merged.heap);

//     return 0;
// }
