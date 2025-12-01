#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>

#define FIFO_PROD_TO_CONS "prod_to_cons"
#define FIFO_CONS_TO_PROD "cons_to_prod"

int main() {
    int shelf_count = 0;
    char buf[20];
    int fd_read, fd_write;
    char input;

    printf("Consumer started. Commands:\n");
    printf("  c - consume\n");
    printf("  q - quit\n");

    // Keep FIFOs open for full lifetime
    fd_read  = open(FIFO_PROD_TO_CONS, O_RDONLY);
    fd_write = open(FIFO_CONS_TO_PROD, O_WRONLY);

    if (fd_read < 0 || fd_write < 0) {
        perror("Failed to open FIFOs");
        exit(1);
    }

    fd_set readfds;

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        FD_SET(fd_read, &readfds);

        int maxfd = (fd_read > STDIN_FILENO ? fd_read : STDIN_FILENO) + 1;
        int activity = select(maxfd, &readfds, NULL, NULL, NULL);

        if (activity < 0) {
            perror("select");
            break;
        }

        // Handle user input
        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            input = getchar();
            if (input == '\n') continue;

            if (input == 'q') {
                write(fd_write, "quit", 5);
                printf("Consumer quitting.\n");
                break;
            } 
            else if (input == 'c') {
                if (shelf_count > 0) {
                    shelf_count--;
                    sprintf(buf, "%d", shelf_count);
                    printf("Consumer: Consumed. Shelf count: %d\n", shelf_count);
                    write(fd_write, buf, strlen(buf)+1);
                } else {
                    strcpy(buf, "empty");
                    printf("Consumer: Shelf empty, cannot consume.\n");
                    write(fd_write, buf, strlen(buf)+1);
                }
            }
        }

        // Handle producer requests
        if (FD_ISSET(fd_read, &readfds)) {
            int n = read(fd_read, buf, sizeof(buf)-1);
            if (n <= 0) {
                // Don’t exit on EOF, just keep looping
                continue;
            }

            buf[n] = '\0';

            if (strncmp(buf, "produce", 7) == 0) {
                if (shelf_count < 5) {
                    shelf_count++;
                    sprintf(buf, "%d", shelf_count);
                    write(fd_write, buf, strlen(buf)+1);
                    printf("Consumer: Item added. Shelf count: %d\n", shelf_count);
                } else {
                    strcpy(buf, "full");
                    write(fd_write, buf, strlen(buf)+1);
                    printf("Consumer: Shelf full. Cannot add.\n");
                }
            }
            else if (strncmp(buf, "quit", 4) == 0) {
                printf("Consumer: Producer quitting, exiting.\n");
                break;
            }
        }
    }

    close(fd_read);
    close(fd_write);

    printf("Consumer exiting.\n");
    return 0;
}

