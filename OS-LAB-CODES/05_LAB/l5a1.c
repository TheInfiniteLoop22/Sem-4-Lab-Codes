// Additional: Create an Orphan Process

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        printf("Child: Before sleep, my PID = %d, parent PID = %d\n", getpid(), getppid());
        sleep(5); // Let parent exit first
        printf("Child: After sleep, my new parent PID = %d (should be 1, init)\n", getppid());
    } else {
        printf("Parent: Exiting immediately (PID = %d)\n", getpid());
        exit(0); // Parent exits
    }
    return 0;
}

