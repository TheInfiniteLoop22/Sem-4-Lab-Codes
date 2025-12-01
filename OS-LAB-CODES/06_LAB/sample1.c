#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    int n, fd[2];
    char buf[100];
    char *data = "hello... this is sample data";
    pipe(fd);
    write(fd[1], data, strlen(data));
    if ((n = read(fd, buf, 1024)) >= 0) {
        buf[n] = 0;
        printf("read %d bytes from the pipe: \"%s\"\n", n, buf);
    } else {
        perror("read");
    }
    exit(0);
}

