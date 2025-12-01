// send_number.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msg_buf {
    long mtype;
    int number;
};

int main() {
    key_t key = 1234;
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }
    struct msg_buf msg;
    msg.mtype = 1;

    printf("Enter a number: ");
    scanf("%d", &msg.number);

    if (msgsnd(msgid, &msg, sizeof(int), 0) == -1) {
        perror("msgsnd failed");
        exit(EXIT_FAILURE);
    }

    printf("Number sent: %d\n", msg.number);
    return 0;
}

