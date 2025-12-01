// check_palindrome.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msg_buf {
    long mtype;
    int number;
};

int is_palindrome(int n) {
    int rev = 0, orig = n;
    while (n > 0) {
        rev = rev * 10 + n % 10;
        n /= 10;
    }
    return rev == orig;
}

int main() {
    key_t key = 1234;
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }
    struct msg_buf msg;

    if (msgrcv(msgid, &msg, sizeof(int), 1, 0) == -1) {
        perror("msgrcv failed");
        exit(EXIT_FAILURE);
    }

    printf("Received number: %d\n", msg.number);
    if (is_palindrome(msg.number))
        printf("The number is a palindrome.\n");
    else
        printf("The number is not a palindrome.\n");

    // Optional: Delete queue after use
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}

