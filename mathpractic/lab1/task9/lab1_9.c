#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#define ARR_SIZE 20

int findMinMax(int *arr) {
    int min = INT_MAX;
    int max = INT_MIN;
    int indMin, indMax;
    for (int i = 0; i < ARR_SIZE; i++) {
        if (arr[i] < min) {
            min = arr[i];
            indMin = i;
        }
        if (arr[i] > max) {
            max = arr[i];
            indMax = i;
        }
    }

    int temp = arr[indMin];
    arr[indMin] = arr[indMax];
    arr[indMax] = temp;

    return 0;
}

int fillArray(int a, int b, int *res, int size) {
    int min_ = a < b ? a : b;
    int lenAB = abs(a - b);

    for (int i = 0; i < size; i++) {
        res[i] = min_ + (rand() % lenAB);
    }
    return 0;
}

int nearestValue(int a, int *arr, size_t size) {
    int nearest = arr[0];
    int minDiff = abs(a - arr[0]);

    for (size_t i = 1; i < size; i++) {
        int diff = abs(a - arr[i]);
        if (diff < minDiff) {
            nearest = arr[i];
            minDiff = diff;
        }
    }

    return nearest;
}

int checkInt(char *n, int *res) {
    char *endptr = NULL;
    long res1 = strtol(n, &endptr, 10);

    if (*endptr != '\0' || strlen(n) > 10 || res1 < INT_MIN || res1 > INT_MAX) {
        return 1;
    }

    *res = (int)res1;
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Wrong argc\n");
        return 1;
    }

    int a, b;
    int checkA = checkInt(argv[1], &a);
    int checkB = checkInt(argv[2], &b);
    if (checkA != 0 || checkB != 0) {
        printf("Invalid input\n");
        return 2;
    }

    int res[ARR_SIZE];
    srand(time(NULL)); 
    fillArray(a, b, res, ARR_SIZE);

    for (int i = 0; i < ARR_SIZE; i++) {
        printf("%d ", res[i]);
    }
    printf("\n");

    findMinMax(res);

    for (int i = 0; i < ARR_SIZE; i++) {
        printf("%d ", res[i]);
    }
    printf("\n");

    // Second task
    int left = 10, right = 10000;
    int sizeA = left + (rand() % (right - left + 1));
    int sizeB = left + (rand() % (right - left + 1));

    int *A = (int *)malloc(sizeof(int) * sizeA);
    int *B = (int *)malloc(sizeof(int) * sizeB);
    int *C = (int *)malloc(sizeof(int) * sizeA);

    if (A == NULL || B == NULL || C == NULL) {
        printf("Memory allocation error\n");
        return 3;
    }

    fillArray(-1000, 1000, A, sizeA);
    fillArray(-1000, 1000, B, sizeB);

    for (int i = 0; i < sizeA; i++) {
        C[i] = A[i] + nearestValue(A[i], B, sizeB);
    }

    
    // for (int i = 0; i < sizeA; i++) {
    //     printf("%d ", A[i]);
    // }
    // printf("\n");

    for (int i = 0; i < sizeA; i++) {
        printf("%d ", C[i]);
    }

    free(A);
    free(B);
    free(C);

    return 0;
}
