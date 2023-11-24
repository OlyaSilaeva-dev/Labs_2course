#include<stdio.h>
#include<unistd.h>

int main(){
    printf("[%d] начали работу\n", getpid());
    
    // fork запускает копию родительского процесса с данной точки выполнения, дочернему fork возвращает 0, родителю возвращает PID  дочернего 
    int pid;
    pid = fork();
    printf("[%d] дочерний %d\n", getpid(), pid);
    
    int pid1 = fork();
    printf("[%d] Hi %d\n", getpid(), pid1);
    int pid2 = fork();
    printf("[%d] Hi %d\n", getpid(), pid2);

    printf("[%d] закончили работу\n", getpid());
    return 0;
}