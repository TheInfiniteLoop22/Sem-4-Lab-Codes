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
        if (shm->available == 1) {
            printf("Message received: %s\n", shm->message);

            // Clear message and reset flag
            memset(shm->message, 0, sizeof(shm->message));
            shm->available = 0;

            // Exit on message "end"
            if (strcmp(shm->message, "end") == 0) {
                break;
            }
        }
        usleep(100000);
    }

    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);  // delete shared memory on exit

    return 0;
}

