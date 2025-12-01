// Create a Child and Display All PIDs

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
        printf("Child: My PID = %d, Parent's PID = %d\n", getpid(), getppid());
    } else {
        printf("Parent: My PID = %d, Child's PID = %d\n", getpid(), pid);
    }
    return 0;
}

