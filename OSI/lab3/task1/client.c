#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/msg.h>

#define MAX_MSG_SIZE sizeof(struct msgbuf)

struct msgbuf {
    long mtype;
    char mtext[BUFSIZ];
};

int main(){
    key_t key;
    int msgid;
    struct msgbuf msg;

    if (key = ftok(".", 'B') == -1){
        return 1;
    }

    if((msgid = msgget(key, 0666)) == -1){
        return 2;
    }

    struct msgbuf messages[] = {
        {5, "prior=0 text=\"Hello World!!\""},
        {3, "prior=0 text=\"Python is my favorite language!!!...\""},
        {1, "prior=5 text=\"Where is my greatest Lab’’’ work??\""},
        {4, "prior=2 text=\" Yes!\""},
        {2, "prior=3 text=\"No……\""},
        {3, "prior=3 text=\"2,,,9\\\"99\\\" .\""},
        {1, "prior=2 text=\"12,,,4 6\""},
        {1, "prior=1 text=\"Terminate\""}
    };

    int num_messages = sizeof(messages) /  sizeof(struct msgbuf);

    for(int i = 0; i < num_messages; i++) {
        if(msgsnd(msgid, (void*)&messages[i], MAX_MSG_SIZE, 0) == -1) {
            return 3;
        }

        if(msgrcv(msgid, (void*)&messages[i], MAX_MSG_SIZE, 0, 0) == -1){
            return 4;
        }   

        printf("Received from server (Priority %ld): %s\n", msg.mtype, msg.mtext);
    }

    return 0;
}
