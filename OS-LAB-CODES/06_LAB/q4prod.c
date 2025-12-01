#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_PROD_TO_CONS "prod_to_cons"
#define FIFO_CONS_TO_PROD "cons_to_prod"

int main() {
    char input;
    char buf[20];
    int fd_write, fd_read;

    printf("Producer started. Commands:\n");
    printf("  p - produce\n");
    printf("  q - quit\n");

    // Keep FIFOs open for full lifetime
    fd_write = open(FIFO_PROD_TO_CONS, O_WRONLY);
    fd_read  = open(FIFO_CONS_TO_PROD, O_RDONLY);

    if (fd_write < 0 || fd_read < 0) {
        perror("Failed to open FIFOs");
        exit(1);
    }

    while (1) {
        input = getchar();
        if (input == '\n') continue;

        if (input == 'q') {
            write(fd_write, "quit", 5);
            break;
        } 
        else if (input == 'p') {
            write(fd_write, "produce", 8);

            int n = read(fd_read, buf, sizeof(buf) - 1);
            if (n > 0) {
                buf[n] = '\0';
                if (strncmp(buf, "full", 4) == 0) {
                    printf("Producer: Shelf full, cannot produce.\n");
                } else {
                    printf("Producer: Produced. Shelf count: %s\n", buf);
                }
            }
        } 
        else {
            printf("Unknown command. Use 'p' to produce, 'q' to quit\n");
        }
    }

    close(fd_write);
    close(fd_read);

    printf("Producer exiting.\n");
    return 0;
}

