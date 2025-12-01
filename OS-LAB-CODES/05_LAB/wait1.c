#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void main() {
    int status;
    pid_t pid;
    pid = fork();  
    if(pid == -1)
        printf("ERROR child not created");
    else if (pid == 0) { // child
        printf("\n I'm the child!");
        exit(0);
    } else { // parent
        wait(&status);
        printf("\n I'm the parent!");
        printf("\n Child returned: %d\n", status);
    }
}

