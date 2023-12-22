#include<stdio.h>
#include<sys/msg.h>
#include<stdint.h>

struct msgbuf{
    char text[BUFSIZ];
    uint32_t msgtype;
};

void process_message(struct msgbuf *msg) {
    // Обработка сообщения - замена разделителей на пробел
    int i;
    for (i = 0; i < strlen(msg->text); ++i) {
        if (msg->text[i] == ' ' || msg->text[i] == '\t') {
            msg->text[i] = ' ';
        }
    }
}


int main(){
    key_t key;
    int msgid;
    struct msgbuf msg;

    if ((key == ftok(".", 'B')) == -1){
        return 1;
    }

    if((msgid = msgget(key, 0666| IPC_CREAT)) == -1){
        return 2;
    }

    while(1){
        if(msgrcv(msgid, (void*)&msg, sizeof(struct msgbuf), 0, 0) == -1){
            printf("Received termination request. Exiting...\n");
            break;
        }

        if(msg.msgtype == 1) {
            printf("Received termination request. Exiting...\n");
            break;
        }

        process_message(&msg);

        if(msgsnd(msgid, (void*)&msg, sizeof(struct msgbuf), 0) == -1){
            return 3;
        }
    }

    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        return 4;
    }

    return 0;
}