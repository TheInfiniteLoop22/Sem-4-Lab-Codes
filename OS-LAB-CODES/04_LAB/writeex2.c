#include <unistd.h>
#include <stdlib.h>

int main() {
    char msg[] = "Hello from write()!\n";
    ssize_t written = write(1, msg, sizeof(msg)-1);

    if (written != sizeof(msg)-1) {
        write(2, "Error in writing output\n", 25);
    }
    return 0;
}

