//1. Block Parent Until Child Completes (using wait)

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
        printf("Child: I am child having id %d\n", getpid());
        printf("Child: My parent's id is %d\n", getppid());
        exit(0);
    } else {
        wait(NULL); // Parent waits for child to finish
        printf("Parent: My child's id is %d\n", pid);
        printf("Parent: I am parent having id %d\n", getpid());
    }
    return 0;
}

