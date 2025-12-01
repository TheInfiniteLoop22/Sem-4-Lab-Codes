#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUF_SIZE 1024
#define LINES_PER_PAGE 20

// Function to wait for a key press
void wait_for_key() {
    char c;
    write(1, "\n-- More -- (press ENTER to continue)\n", 38);
    read(0, &c, 1); // waits for user input
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        write(2, "Usage: ./mymore <file1> [file2 ...]\n", 36);
        exit(1);
    }

    char buf[BUF_SIZE];
    int fd, bytes, i;
    int line_cnt, line_num;

    for (int fileidx = 1; fileidx < argc; fileidx++) {
        fd = open(argv[fileidx], O_RDONLY);
        if (fd == -1) {
            perror("open");
            continue;
        }

        line_cnt = 0;
        line_num = 1; // start numbering from 1

        // Print number for very first line before reading any data
        char numbuf[32];
        int numlen = snprintf(numbuf, sizeof(numbuf), "%4d: ", line_num++);
        write(1, numbuf, numlen);

        while ((bytes = read(fd, buf, BUF_SIZE)) > 0) {
            for (i = 0; i < bytes; i++) {
                write(1, &buf[i], 1); // print character

                if (buf[i] == '\n') {
                    line_cnt++;

                    // pause every 20 lines
                    if (line_cnt == LINES_PER_PAGE) {
                        wait_for_key();
                        line_cnt = 0;
                    }

                    // print the next line number
                    numlen = snprintf(numbuf, sizeof(numbuf), "%4d: ", line_num++);
                    write(1, numbuf, numlen);
                }
            }
        }
        close(fd);
    }

    return 0;
}

