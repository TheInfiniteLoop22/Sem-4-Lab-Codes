#include <unistd.h>
#include <stdlib.h>

int main() {
    char buffer[128];
    int nread;

    nread = read(0, buffer, 128); // from STDIN
    if (nread == -1)
        write(2, "A read error has occurred\n", 26);
    if (write(1, buffer, nread) != nread) // write to STDOUT
        write(2, "A write error has occurred\n", 27);
    exit(0);
}

