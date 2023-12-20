#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef enum status_code {
    OK = 0,
    WRONG_ARGC = 2,
} status_code;

enum BathroomState {
    NO_ONE,
    ONLY_WOMEN,
    ONLY_MEN
};

int womenCount = 0;
int menCount = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t womenCanEnter = PTHREAD_COND_INITIALIZER;
pthread_cond_t menCanEnter = PTHREAD_COND_INITIALIZER;

void woman_wants_to_enter() {
    pthread_mutex_lock(&mutex);

    while (menCount > 0 || womenCount > 0) {
        pthread_cond_wait(&womenCanEnter, &mutex);
    }

    womenCount++;

    printf("Женщина входит в ванную комнату\n");

    pthread_mutex_unlock(&mutex);
}

void man_wants_to_enter() {
    pthread_mutex_lock(&mutex);

    while (womenCount > 0 || menCount > 0) {
        pthread_cond_wait(&menCanEnter, &mutex);
    }

    menCount++;

    printf("Мужчина входит в ванную комнату\n");

    pthread_mutex_unlock(&mutex);
}

void woman_leaves() {
    pthread_mutex_lock(&mutex);

    womenCount--;

    if (womenCount == 0) {
        pthread_cond_signal(&menCanEnter);
    }

    printf("Женщина покидает ванную комнату\n");

    pthread_mutex_unlock(&mutex);
}

void man_leaves() {
    pthread_mutex_lock(&mutex);

    menCount--;

    if (menCount == 0) {
        pthread_cond_signal(&womenCanEnter);
    }

    printf("Мужчина покидает ванную комнату\n");

    pthread_mutex_unlock(&mutex);
}

void *womenThreadFunction(void *arg) {
    for (int i = 0; i < *((int *)arg); i++) {
        woman_wants_to_enter();
        woman_leaves();
    }
    return NULL;
}

void *menThreadFunction(void *arg) {
    for (int i = 0; i < *((int *)arg); i++) {
        man_wants_to_enter();
        man_leaves();
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Wrong argc");
        return WRONG_ARGC;
    }

    int N = atoi(argv[1]);

    pthread_t womenThread, menThread;

    // Запуск потоков с использованием максимального числа человек (N)
    pthread_create(&womenThread, NULL, womenThreadFunction, &N);
    pthread_create(&menThread, NULL, menThreadFunction, &N);

    pthread_join(womenThread, NULL);
    pthread_join(menThread, NULL);

    return OK;
}
