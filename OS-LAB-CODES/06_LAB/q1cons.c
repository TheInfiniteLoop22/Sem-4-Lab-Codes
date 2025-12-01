#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define FIFO_NAME "myfifo"

int main() {
    int fd;
    int numbers[4];

    // Open FIFO for reading
    fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Read 4 integers from the FIFO
    read(fd, numbers, sizeof(numbers));

    printf("Consumer: Received numbers from FIFO:\n");
    for (int i = 0; i < 4; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");

    close(fd);
    return 0;
}

