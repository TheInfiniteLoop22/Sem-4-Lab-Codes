#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "Fork Failed");
        exit(-1);
    } else if (pid == 0) { // child
        execlp("/bin/ls", "ls", NULL);
    } else { // parent
        wait (NULL);
        printf ("Child Complete");
        exit(0);
    }
}

