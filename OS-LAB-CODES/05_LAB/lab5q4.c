// Create a Zombie (defunct) Process

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
        printf("Child: Exiting to become zombie (PID = %d)\n", getpid());
        exit(0); // Child exits immediately
    } else {
        printf("Parent: Sleeping, child will be zombie for a while (Parent PID = %d)\n", getpid());
        sleep(5); // Parent sleeps, during which child is zombie
        // Run 'ps' while parent sleeps to observe zombie status
    }
    return 0;
}

