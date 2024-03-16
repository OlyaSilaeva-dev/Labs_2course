#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>

typedef enum status_codes { 
    OK = 0,
    WRONG_ARGC = 1,
} status_codes;

#define N_MAN 20 // Всего мужчин
#define N_WOMAN 20 // Всего женщин

// Вместимость ванной
int n_max;
// Кол-во мужчин/женщин в ванной
int n_man = 0;
int n_woman = 0;
// Мьютекс для проверки ванной
pthread_mutex_t mt;
// Флаг для одновременного начала работы
bool start_flag = false;

char* get_time(char *str) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf (str, "%02d.%02d.%d %02d:%02d:%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    return str;
}

bool woman_wants_to_enter(int my_index) {
    bool result;
    char str_time[80];
    
    pthread_mutex_lock(&mt);
    if ((n_man == 0) && (n_woman < n_max)) {
        n_woman++;
        printf("[%s] Woman %d enter. Total in bath: man:%d; woman:%d\n", get_time(str_time), my_index, n_man, n_woman);
        result = true;
    } else {
        result = false;
    }
    pthread_mutex_unlock(&mt);
    return result;
}

void woman_leave(int my_index) {
    char str_time[80];
    
    pthread_mutex_lock(&mt);
    n_woman--;
    printf("[%s] Woman %d leave. Total in bath: man:%d; woman:%d\n", get_time(str_time), my_index, n_man, n_woman);
    pthread_mutex_unlock(&mt);
}

bool man_wants_to_enter(int my_index) {
    bool result;
    char str_time[80];
    
    pthread_mutex_lock(&mt);
    if ((n_woman == 0) && (n_man < n_max)) {
        n_man++;
        printf("[%s] Man %d enter. Total in bath: man:%d; woman:%d\n", get_time(str_time), my_index, n_man, n_woman);
        result = true;
    } else {
        result = false;
    }
    pthread_mutex_unlock(&mt);
    return result;
}

void man_leave(int my_index) {
    char str_time[80];

    pthread_mutex_lock(&mt);
    n_man--;
    printf("[%s] Man %d leave. Total in bath: man:%d; woman:%d\n", get_time(str_time), my_index, n_man, n_woman);
    pthread_mutex_unlock(&mt);
}

void* man_execute(void *args) {
    int my_index = *(int *)args;
    while (!start_flag) {
        sleep(1);
    }
    while (!man_wants_to_enter(my_index)) {
        sleep(1);
    }
    sleep(1);
    man_leave(my_index);
    return NULL;
}

void* woman_execute(void *args) {
    int my_index = *(int *)args;
    while (!start_flag) {
        sleep(1);
    }
    while (!woman_wants_to_enter(my_index)) {
        sleep(1);
    }
    sleep(1);
    woman_leave(my_index);
    return NULL;
}


void main_process(int n_max_val) {
    n_max = n_max_val;
    
    pthread_mutex_init(&mt, NULL);
    
    pthread_t man_threads[N_MAN];
    pthread_t woman_threads[N_WOMAN];
    int id_man[N_MAN];
    int id_woman[N_WOMAN];
    
    for (int i = 0; i < N_MAN; i++) {
        id_man[i] = i + 1;
        pthread_create(&man_threads[i], NULL, man_execute, &id_man[i]);
    }
    for (int i = 0; i < N_WOMAN; i++) {
        id_woman[i] = i + 1;
        pthread_create(&woman_threads[i], NULL, woman_execute, &id_woman[i]);
    }

    start_flag = true;

    // ожидаем завершения
    for (int i = 0; i < N_MAN; i++) {
        pthread_join(man_threads[i], NULL);
    }
    for (int i = 0; i < N_WOMAN; i++) {
        pthread_join(woman_threads[i], NULL);
    }
    
    pthread_mutex_destroy(&mt);
}

status_codes main(int argc, char * argv[]) {
    if(argc != 2) {
        printf("Wrong argc");
        return WRONG_ARGC;
    }
    char str_time[80];

    if (argc == 1) {
        printf("Command line has no additional arguments\n");
        return 0;
    }
    int n_max_bath = (int) strtol(argv[1], NULL, 10);
    if (n_max_bath <= 0) {
        printf("Wrong arguments\n");
        return 0;
    }
    printf("[%s] Start process wth N=%d\n", get_time(str_time), n_max_bath);
    main_process(n_max_bath);
    printf("[%s] End process\n", get_time(str_time));

    return OK;
}