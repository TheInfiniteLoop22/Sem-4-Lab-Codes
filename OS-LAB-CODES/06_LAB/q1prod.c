#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define FIFO_NAME "myfifo"

int main() {
    int fd;
    int numbers[4] = {10, 20, 30, 40};

    // Create the FIFO if it doesn't exist
    mkfifo(FIFO_NAME, 0666);

    // Open FIFO for writing
    fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Write 4 integers into the FIFO
    write(fd, numbers, sizeof(numbers));

    printf("Producer: Written numbers to FIFO\n");

    close(fd);
    return 0;
}

