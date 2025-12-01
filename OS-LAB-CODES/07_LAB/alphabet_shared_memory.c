#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SHM_SIZE sizeof(struct shm_alphabet)

struct shm_alphabet {
    char alphabet;
    char next_alphabet;
    int ready; // 0: not ready, 1: child ready for reading/writing, 2: parent ready
};

int main() {
    key_t key = 1234;
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    struct shm_alphabet* shm_ptr = (struct shm_alphabet*) shmat(shmid, NULL, 0);
    if (shm_ptr == (void*) -1) {
        perror("shmat failed");
        shmctl(shmid, IPC_RMID, NULL);
        exit(EXIT_FAILURE);
    }

    shm_ptr->ready = 0; // synchronize

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        shmdt(shm_ptr);
        shmctl(shmid, IPC_RMID, NULL);
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        // Child process
        while (shm_ptr->ready == 0) {
            usleep(100000);
        }
        char c = shm_ptr->alphabet;  // get single char value correctly
        printf("Child received: %c\n", c);

        if (c >= 'A' && c < 'Z')
            shm_ptr->next_alphabet = c + 1;
        else if (c == 'Z')
            shm_ptr->next_alphabet = 'A';
        else if (c >= 'a' && c < 'z')
            shm_ptr->next_alphabet = c + 1;
        else if (c == 'z')
            shm_ptr->next_alphabet = 'a';
        else
            shm_ptr->next_alphabet = '?'; // invalid input

        shm_ptr->ready = 2; // signal completed
        shmdt(shm_ptr);
        exit(EXIT_SUCCESS);
    }
    else {
        // Parent process
        printf("Enter an English alphabet: ");
        char input[5];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Failed to read input\n");
            shmdt(shm_ptr);
            shmctl(shmid, IPC_RMID, NULL);
            exit(EXIT_FAILURE);
        }

        shm_ptr->alphabet = input[0];  // assign first character, not string pointer
        shm_ptr->ready = 1; // signal to child

        while (shm_ptr->ready != 2) {
            usleep(100000);
        }

        printf("Parent received reply: %c\n", shm_ptr->next_alphabet);

        wait(NULL); // wait for child to finish

        shmdt(shm_ptr);
        shmctl(shmid, IPC_RMID, NULL);
        return 0;
    }
}

