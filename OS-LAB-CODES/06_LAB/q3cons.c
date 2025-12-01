//If implementing the Consumer (reads from FIFO)
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define FIFO_NAME "/tmp/my_fifo"

int main() {
    int fd, arr[13];
    fd = open(FIFO_NAME, O_RDONLY);      // Open FIFO for reading
    read(fd, arr, sizeof(arr));
    printf("Consumer received: %d %d %d %d\n", arr, arr[1], arr[2], arr[14]);
    close(fd);
    return 0;
}

