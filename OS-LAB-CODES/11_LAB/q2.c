/*
2. Write a multithreaded program that implements the banker's algorithm. Create n threads that request and release resources from the bank. The banker will grant the request only if it leaves the system in a safe state. You may write this program using pthreads. It is important that shared data be safe from concurrent access. To ensure safe access.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define P 5  // Number of processes (threads)
#define R 3  // Number of resource types

// Shared data structures
int available[R] = {3, 3, 2};
int max[P][R] = {
    {7, 5, 3},
    {3, 2, 2},
    {9, 0, 2},
    {2, 2, 2},
    {4, 3, 3}
};
int allocation[P][R] = {
    {0, 1, 0},
    {2, 0, 0},
    {3, 0, 2},
    {2, 1, 1},
    {0, 0, 2}
};
int need[P][R];

pthread_mutex_t lock;

// Function to calculate the need matrix
void calculateNeed() {
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            need[i][j] = max[i][j] - allocation[i][j];
}

// Banker's safety check
bool isSafe() {
    int work[R];
    bool finish[P];
    for (int i = 0; i < R; i++)
        work[i] = available[i];
    for (int i = 0; i < P; i++)
        finish[i] = false;

    int count = 0;
    while (count < P) {
        bool found = false;
        for (int p = 0; p < P; p++) {
            if (!finish[p]) {
                bool canProceed = true;
                for (int r = 0; r < R; r++) {
                    if (need[p][r] > work[r]) {
                        canProceed = false;
                        break;
                    }
                }
                if (canProceed) {
                    for (int r = 0; r < R; r++)
                        work[r] += allocation[p][r];
                    finish[p] = true;
                    found = true;
                    count++;
                }
            }
        }
        if (!found)
            return false;
    }
    return true;
}

// Function to request resources by process pid
bool requestResources(int pid, int request[R]) {
    pthread_mutex_lock(&lock);

    // Check if request <= need
    for (int i = 0; i < R; i++) {
        if (request[i] > need[pid][i]) {
            printf("Process %d requested more than its maximum need.\n", pid);
            pthread_mutex_unlock(&lock);
            return false;
        }
    }
    // Check if request <= available
    for (int i = 0; i < R; i++) {
        if (request[i] > available[i]) {
            printf("Resources not available for Process %d.\n", pid);
            pthread_mutex_unlock(&lock);
            return false;
        }
    }

    // Pretend to allocate
    for (int i = 0; i < R; i++) {
        available[i] -= request[i];
        allocation[pid][i] += request[i];
        need[pid][i] -= request[i];
    }

    if (isSafe()) {
        printf("Request granted to Process %d\n", pid);
        pthread_mutex_unlock(&lock);
        return true;
    } else {
        // Rollback allocation
        for (int i = 0; i < R; i++) {
            available[i] += request[i];
            allocation[pid][i] -= request[i];
            need[pid][i] += request[i];
        }
        printf("Request denied for Process %d to maintain safe state.\n", pid);
        pthread_mutex_unlock(&lock);
        return false;
    }
}

// Function to release resources
void releaseResources(int pid, int release[R]) {
    pthread_mutex_lock(&lock);
    for (int i = 0; i < R; i++) {
        // Ensure not releasing more than allocated
        if (release[i] > allocation[pid][i])
            release[i] = allocation[pid][i];
        allocation[pid][i] -= release[i];
        available[i] += release[i];
        need[pid][i] += release[i];
    }
    printf("Process %d released resources.\n", pid);
    pthread_mutex_unlock(&lock);
}

// Thread function simulating process behavior
void* process(void* arg) {
    int pid = *(int*)arg;
    free(arg);
    printf("Process %d started\n", pid);

    // Random requests/releases for demonstration:
    for (int i = 0; i < 3; i++) {
        int request[R] = {rand() % (need[pid][0] + 1), rand() % (need[pid][1] + 1), rand() % (need[pid][2] + 1)};
        if ((request[0] + request[1] + request[2]) == 0)
            continue; // Skip zero request

        if (requestResources(pid, request)) {
            // Hold resources for a random time
            sleep(1);

            // Release some or all of allocated resources randomly
            int release[R] = {rand() % (allocation[pid][0] + 1), rand() % (allocation[pid][1] + 1), rand() % (allocation[pid][2] + 1)};
            releaseResources(pid, release);
        }
        sleep(1);
    }
    printf("Process %d finished\n", pid);
    return NULL;
}

int main() {
    pthread_t threads[P];
    pthread_mutex_init(&lock, NULL);
    calculateNeed();

    // Seed random
    srand(time(NULL));

    for (int i = 0; i < P; i++) {
        int* pid = malloc(sizeof(int));
        *pid = i;
        pthread_create(&threads[i], NULL, process, pid);
    }

    for (int i = 0; i < P; i++)
        pthread_join(threads[i], NULL);

    pthread_mutex_destroy(&lock);
    printf("All processes have finished.\n");

    return 0;
}

/*

Sample Output (will vary slightly due to randomness):

-----------------------------------------------------------------------------------
all 5 threads (processes) have begun execution concurrently. 
-----------------------------------------------------------------------------------
Process 0 started
Process 1 started
Process 2 started
Process 3 started
Process 4 started
-----------------------------------------------------------------------------------
process made a resource request, and the Banker's Algorithm found the system remains in a safe state after granting. Therefore, the requests were allowed.
-----------------------------------------------------------------------------------
Request granted to Process 0
Request granted to Process 1
Request granted to Process 3
Request granted to Process 2
Request granted to Process 4
-----------------------------------------------------------------------------------
After holding the granted resources for some time (simulated by sleep), each process releases some or all allocated resources, updating shared resource availability.
-----------------------------------------------------------------------------------
Process 0 released resources.
Process 1 released resources.
Process 3 released resources.
Process 2 released resources.
Process 4 released resources.
-----------------------------------------------------------------------------------
Processes 0, 1, 3, and 4 have their requests granted because granting them maintains a safe system state.

Process 2's request is denied because granting it would risk resource exhaustion or lead to an unsafe state where deadlock might occur.
-----------------------------------------------------------------------------------
Request granted to Process 0
Request granted to Process 1
Request granted to Process 3
Request denied for Process 2 to maintain safe state.
Request granted to Process 4
-----------------------------------------------------------------------------------
More Releases and Process Completion
-----------------------------------------------------------------------------------
Process 0 released resources.
Process 1 released resources.
Process 3 released resources.
Process 4 released resources.
Process 0 finished
Process 1 finished
Process 3 finished
Process 4 finished
Process 2 finished
All processes have finished.

*/

