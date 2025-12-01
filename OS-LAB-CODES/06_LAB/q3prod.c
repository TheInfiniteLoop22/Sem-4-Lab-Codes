// If implementing the Producer (writes to FIFO):
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_NAME "/tmp/my_fifo"

int main() {
    int fd;
    mkfifo(FIFO_NAME, 0666);             // Create FIFO named pipe
    fd = open(FIFO_NAME, O_WRONLY);      // Open for writing
    int arr[4] = {1, 2, 3, 4};
    write(fd, arr, sizeof(arr));         // Send 4 integers
    printf("Producer sent: %d %d %d %d\n", arr[0], arr[1], arr[2], arr[3]);
    close(fd);
    // unlink(FIFO_NAME); // Optional: remove FIFO after use
    return 0;
}

