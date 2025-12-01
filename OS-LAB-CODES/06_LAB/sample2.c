#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char *argv[]) {
    int pfd[2];
    pid_t cpid;
    char buf;
    pipe(pfd);
    cpid = fork();
    if (cpid == 0) { // Child
        close(pfd[1]);
        while (read(pfd, &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);
        write(STDOUT_FILENO, "\n", 1);
        close(pfd);
        exit(EXIT_SUCCESS);
    } else { // Parent
        close(pfd);
        write(pfd[1], argv[1], strlen(argv[1]));
        close(pfd[1]);
        wait(NULL);
        exit(EXIT_SUCCESS);
    }
}

