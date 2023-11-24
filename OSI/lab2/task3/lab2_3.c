#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define N 5 // Количество философов

sem_t forks[N]; 

void *philosopher(void *num) {
    int id = *(int *)num;
    int left_fork = id;
    int right_fork = (id + 1) % N;

    while (1) {
        // Думаем
        printf("Философ %d думает\n", id);

        // Голодаем
        printf("Философ %d голоден\n", id);

        // Блокируем вилки
        if (id % 2 == 0) {
            sem_wait(&forks[left_fork]);
            printf("Философ %d взял левую вилку\n", id);
            sem_wait(&forks[right_fork]);
            printf("Философ %d взял правую вилку\n", id);
        } else {
            sem_wait(&forks[left_fork]);
            printf("Философ %d взял левую вилку\n", id);
            sem_wait(&forks[right_fork]);
            printf("Философ %d взял правую вилку\n", id);
        }

        // Едим
        printf("Философ %d ест\n", id);
        sleep(1);

        // Освобождаем вилки
        sem_post(&forks[left_fork]);
        printf("Философ %d освободил левую вилку\n", id);
        sem_post(&forks[right_fork]);
        printf("Философ %d освободил правую вилку\n", id);
    }
    return NULL;
}

int main() {
    pthread_t philosophers[N];
    int id[N];

    // Инициализация семафоров
    for (int i = 0; i < N; i++) {
        sem_init(&forks[i], 0, 1);
    }

    // Создание потоков философов
    for (int i = 0; i < N; i++) {
        id[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &id[i]);
    }

    // Ожидание завершения потоков
    for (int i = 0; i < N; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Уничтожение семафоров
    for (int i = 0; i < N; i++) {
        sem_destroy(&forks[i]);
    }

    return 0;
}
