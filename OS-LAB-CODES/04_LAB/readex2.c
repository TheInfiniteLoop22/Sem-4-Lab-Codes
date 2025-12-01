#include <unistd.h>

int main() {
    char buf[10];
    int bytes = read(0, buf, sizeof(buf)); // read up to 10 chars
    write(1, buf, bytes); // echo it back
    return 0;
}

