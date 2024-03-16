#include "../header.h"

typedef struct BinaryHeap {
    int* heap;
    int size;
} BinaryHeap;

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
    pq->heap[pq->size++] = item;
    heapyfy_up(pq, pq->size - 1);
}

int extract_max(BinaryHeap *pq) { //возвращает элемент с максимальным приоритетом, заменяет корневой элемент последним элементом кучи
    if (pq->size == 0) {
        return -1;//pq underflow
    }
    int max_item = pq->heap[0];
    pq->heap[0] = pq->heap[pq->size-1];
    pq->size--;
    heapify_down(pq, 0);
    return max_item;
}

int peek_max(BinaryHeap *pq) { //возвращает элемент с макс. приоритетом не удаляя его из очереди
    if(pq->size == 0) {
        return -1;//pa is empty
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
