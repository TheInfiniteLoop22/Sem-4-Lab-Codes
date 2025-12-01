#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_KEY 1234
#define SHM_SIZE 1024

struct shared_data {
    int available;      // 0 = no new message, 1 = new message available
    char message[SHM_SIZE - sizeof(int)];
};

int main() {
    int shmid = shmget(SHM_KEY, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    struct shared_data *shm = (struct shared_data *) shmat(shmid, NULL, 0);
    if (shm == (void*) -1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Enter your message: ");
        if (fgets(shm->message, sizeof(shm->message), stdin) == NULL) {
            perror("fgets failed");
            break;
        }
        // Remove trailing newline
        size_t len = strlen(shm->message);
        if (len > 0 && shm->message[len - 1] == '\n') {
            shm->message[len - 1] = '\0';
        }

        shm->available = 1;  // indicate new message available

        // Wait until Display clears the message
        while (shm->available == 1) {
            usleep(100000);
        }

        // Optional: exit if message is "end"
        if (strcmp(shm->message, "end") == 0) {
            break;
        }
    }

    shmdt(shm);
    // Do not delete shared memory here; Display program can delete on exit

    return 0;
}

