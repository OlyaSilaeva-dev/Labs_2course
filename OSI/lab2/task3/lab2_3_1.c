#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define N 5 // Количество философов
#define MAX_ALLOWED_PHILOSOPHERS N-1 // Максимальное количество философов, которым разрешено есть одновременно

sem_t forks[N];
sem_t waiter; // Официант

void *philosopher(void *num) {
    int id = *(int *)num;
    int left_fork = id;
    int right_fork = (id + 1) % N;

    while (1) {
        // Думаем
        printf("Философ %d думает\n", id);

        // Голодаем
        printf("Философ %d голоден\n", id);

        // Запросить разрешение у официанта
        sem_wait(&waiter);

        // Блокируем вилки
        sem_wait(&forks[left_fork]);
        printf("Философ %d взял левую вилку\n", id);
        sem_wait(&forks[right_fork]);
        printf("Философ %d взял правую вилку\n", id);

        // Освободить официанта
        sem_post(&waiter);

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
    sem_init(&waiter, 0, MAX_ALLOWED_PHILOSOPHERS);
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
    sem_destroy(&waiter);

    return 0;
}
