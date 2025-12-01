// Additional: Wait and Display Child’s Exit Status

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int status;
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        printf("Child: I am exiting with code 42\n");
        exit(42);
    } else {
        wait(&status);
        if (WIFEXITED(status)) {
            printf("Parent: Child exited with status=%d\n", WEXITSTATUS(status));
        }
    }
    return 0;
}

