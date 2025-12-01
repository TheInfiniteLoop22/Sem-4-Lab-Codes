#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int pipefd[2], nums[] = {5, 15, 25, 35}, recv[4];
    if (pipe(pipefd) == -1) exit(1);
    if (fork()) {
        close(pipefd[0]);
        write(pipefd[1], nums, sizeof(nums));
        close(pipefd[1]);
        printf("Parent sent numbers\n");
    } else {
        close(pipefd[1]);
        read(pipefd[0], recv, sizeof(recv));
        close(pipefd[0]);
        printf("Child received:");
        for (int i = 0; i < 4; i++) printf(" %d", recv[i]);
        printf("\n");
    }
    return 0;
}

