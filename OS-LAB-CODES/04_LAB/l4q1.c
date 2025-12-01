//Write a program to print the lines of a file that contain a word given as the program argument (a simple version of the grep UNIX utility).
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        write(2, "Usage: ./mygrep <word> <filename>\n", 34);
        exit(1);
    }

    char *word = argv[1];
    char *filename = argv[2];
    char buffer[BUF_SIZE];
    char line[BUF_SIZE];
    int fd, bytes_read, idx = 0;

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(1);
    }

    while ((bytes_read = read(fd, buffer, BUF_SIZE)) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n' || idx >= BUF_SIZE - 1) {
                line[idx] = '\0'; // terminate string
                if (strstr(line, word) != NULL) { // word found
                    write(1, line, strlen(line));
                    write(1, "\n", 1);
                }
                idx = 0; // reset for next line
            } else {
                line[idx++] = buffer[i];
            }
        }
    }

    close(fd);
    return 0;
}

