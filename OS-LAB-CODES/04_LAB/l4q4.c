#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file> <dest_file>\n", argv[0]);
        return 1;
    }

    FILE *src = fopen(argv[1], "r");
    if (!src) {
        perror("Error opening source file");
        return 1;
    }

    FILE *dest = fopen(argv[2], "w");
    if (!dest) {
        perror("Error opening destination file");
        fclose(src);
        return 1;
    }

    int ch;
    while ((ch = fgetc(src)) != EOF) {
        fputc(ch, dest);
    }

    fclose(src);
    fclose(dest);

    printf("File copied successfully.\n");
    return 0;
}

