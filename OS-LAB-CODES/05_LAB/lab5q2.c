#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Execute your own compiled program
        execlp("./l5q1", "l5q1", NULL);
        perror("execlp failed");
        exit(1);
    } else {
        wait(NULL); // Parent waits for child to finish
        printf("Parent: Child executed l5q1 program\n");
    }
    return 0;
}

