/*
1. Consider the following snapshot of the system. Write C program to implement Banker’s algorithm for deadlock avoidance. The program has to accept all inputs from the user.
Assume the total number of instances of A,B and C are 10,5 and 7 respectively.
(a) What is the content of the matrix Need?
(b) Is the system in a safe state?
(c) If a request from process P1 arrives for (1, 0, 2), can the request be granted
immediately? Display the updated Allocation, Need and Available matrices.
(d) If a request from process P4 arrives for (3, 3, 0), can the request be granted
immediately?
(e) If a request from process P0 arrives for (0, 2, 0), can the request be granted
immediately?

Allocation      Max      Available
    A B C       A B C       A B C
p₀  0 1 0       7 5 3       3 3 2
p₁  2 0 0       3 2 2
p₂  3 0 2       9 0 2
p₃  2 1 1       2 2 2
p₄  0 0 2       4 3 3
*/

#include <stdio.h>
#include <stdbool.h>

#define P 5  // Number of processes
#define R 3  // Number of resource types

void calculateNeed(int need[P][R], int max[P][R], int allocation[P][R]) {
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            need[i][j] = max[i][j] - allocation[i][j];
}

bool checkSafe(int allocation[P][R], int need[P][R], int available[R], int safeSeq[P]) {
    int work[R];
    bool finish[P] = {false};
    for (int i = 0; i < R; i++) work[i] = available[i];

    int count = 0;
    while (count < P) {
        bool found = false;
        for (int p = 0; p < P; p++) {
            if (!finish[p]) {
                bool canAllocate = true;
                for (int r = 0; r < R; r++)
                    if (need[p][r] > work[r]) {
                        canAllocate = false;
                        break;
                    }
                if (canAllocate) {
                    for (int r = 0; r < R; r++)
                        work[r] += allocation[p][r];
                    safeSeq[count++] = p;
                    finish[p] = true;
                    found = true;
                }
            }
        }
        if (!found) return false;
    }
    return true;
}

bool requestResources(int p, int request[R], int allocation[P][R], int need[P][R], int available[R]) {
    // Check if request <= need
    for (int i = 0; i < R; i++) {
        if (request[i] > need[p][i]) {
            printf("Error: Process %d has exceeded its maximum claim.\n", p);
            return false;
        }
    }
    // Check if request <= available
    for (int i = 0; i < R; i++) {
        if (request[i] > available[i]) {
            printf("Resources unavailable for process %d.\n", p);
            return false;
        }
    }
    // Tentatively allocate
    for (int i = 0; i < R; i++) {
        available[i] -= request[i];
        allocation[p][i] += request[i];
        need[p][i] -= request[i];
    }

    int safeSeq[P];
    bool safe = checkSafe(allocation, need, available, safeSeq);
    if (!safe) {
        // Rollback
        for (int i = 0; i < R; i++) {
            available[i] += request[i];
            allocation[p][i] -= request[i];
            need[p][i] += request[i];
        }
        return false;
    }
    return true;
}

void printMatrix(const char *name, int matrix[P][R]) {
    printf("%s Matrix:\n", name);
    printf("A B C\n");
    for (int i = 0; i < P; i++) {
        printf("P%d ", i);
        for (int j = 0; j < R; j++)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
    printf("\n");
}

void printAvailable(int available[R]) {
    printf("Available Resources:\n");
    printf("A B C\n");
    for (int i = 0; i < R; i++)
        printf("%d ", available[i]);
    printf("\n\n");
}

int main() {
    int allocation[P][R] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };
    int maximum[P][R] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };
    int available[R] = {3, 3, 2};

    int need[P][R];
    calculateNeed(need, maximum, allocation);

    printMatrix("Allocation", allocation);
    printMatrix("Maximum", maximum);
    printMatrix("Need", need);
    printAvailable(available);

    int safeSeq[P];
    bool safe = checkSafe(allocation, need, available, safeSeq);
    if (safe) {
        printf("System is in a safe state.\nSafe sequence is: ");
        for (int i = 0; i < P; i++)
            printf("P%d ", safeSeq[i]);
        printf("\n");
    } else {
        printf("System is NOT in a safe state.\n");
    }

    // Requests to test
    int requests[3][R] = {
        {1, 0, 2}, // P1
        {3, 3, 0}, // P4
        {0, 2, 0}  // P0
    };
    int processes[3] = {1, 4, 0};

    for (int i = 0; i < 3; i++) {
        int p = processes[i];
        printf("\nRequest from process P%d for ", p);
        printf("(%d, %d, %d)\n", requests[i][0], requests[i][1], requests[i][2]);

        if (requestResources(p, requests[i], allocation, need, available)) {
            printf("Request can be granted immediately.\n");
            printMatrix("Updated Allocation", allocation);
            printMatrix("Updated Need", need);
            printAvailable(available);
        } else {
            printf("Request cannot be granted immediately.\n");
        }
    }

    return 0;
}

/*
Sample Output:

Allocation Matrix:
A B C
P0 0 1 0
P1 2 0 0
P2 3 0 2
P3 2 1 1
P4 0 0 2

Maximum Matrix:
A B C
P0 7 5 3
P1 3 2 2
P2 9 0 2
P3 2 2 2
P4 4 3 3

Need Matrix:
A B C
P0 7 4 3
P1 1 2 2
P2 6 0 0
P3 0 1 1
P4 4 3 1

Available Resources:
A B C
3 3 2

System is in a safe state.
Safe sequence is: P1 P3 P4 P0 P2 

Request from process P1 for (1, 0, 2)
Request can be granted immediately.
Updated Allocation Matrix:
A B C
P0 0 1 0
P1 3 0 2
P2 3 0 2
P3 2 1 1
P4 0 0 2

Updated Need Matrix:
A B C
P0 7 4 3
P1 0 2 0
P2 6 0 0
P3 0 1 1
P4 4 3 1

Available Resources:
A B C
2 3 0

Request from process P4 for (3, 3, 0)
Request cannot be granted immediately.

Request from process P0 for (0, 2, 0)
Request can be granted immediately.
Updated Allocation Matrix:
A B C
P0 0 3 0
P1 2 0 0
P2 3 0 2
P3 2 1 1
P4 0 0 2

Updated Need Matrix:
A B C
P0 7 2 3
P1 1 2 2
P2 6 0 0
P3 0 1 1
P4 4 3 1

Available Resources:
A B C
3 1 2

*/

