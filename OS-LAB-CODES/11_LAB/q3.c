/*
3. Simulate implementation of Disk Scheduling Algorithms: FCFS, SSTF using a
structure DSA. An DSA contains the request ID, arrival timestamp, cylinder, address,
and the ID of the process that posted the request.
struct DSA {
int request_id;
int arrival_time_stamp;
int cylinder;
int address;
int process_id;
}
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int request_id;
    int arrival_time_stamp;
    int cylinder;
    int address;
    int process_id;
} DSA;

int compareArrival(const void* a, const void* b) {
    DSA* reqA = (DSA*)a;
    DSA* reqB = (DSA*)b;
    return (reqA->arrival_time_stamp - reqB->arrival_time_stamp);
}

void FCFS(DSA requests[], int n, int initial_head) {
    printf("FCFS Disk Scheduling:\n");
    int head = initial_head;
    int total_seek = 0;
    for (int i = 0; i < n; i++) {
        int seek = abs(requests[i].cylinder - head);
        total_seek += seek;
        printf("Servicing request %d (Process %d) at cylinder %d: head moves from %d to %d, seek = %d\n",
               requests[i].request_id, requests[i].process_id, requests[i].cylinder, head, requests[i].cylinder, seek);
        head = requests[i].cylinder;
    }
    printf("Total Seek Time for FCFS = %d\n\n", total_seek);
}

void SSTF(DSA requests[], int n, int initial_head) {
    printf("SSTF Disk Scheduling:\n");
    int total_seek = 0;
    int head = initial_head;
    int serviced[n];
    for (int i = 0; i < n; i++) serviced[i] = 0;
    int serviced_count = 0;

    while (serviced_count < n) {
        int nearest = -1;
        int min_distance = __INT_MAX__;
        for (int i = 0; i < n; i++) {
            if (!serviced[i]) {
                int dist = abs(requests[i].cylinder - head);
                if (dist < min_distance) {
                    min_distance = dist;
                    nearest = i;
                }
            }
        }
        serviced[nearest] = 1;
        total_seek += min_distance;
        printf("Servicing request %d (Process %d) at cylinder %d: head moves from %d to %d, seek = %d\n",
               requests[nearest].request_id, requests[nearest].process_id, requests[nearest].cylinder,
               head, requests[nearest].cylinder, min_distance);
        head = requests[nearest].cylinder;
        serviced_count++;
    }
    printf("Total Seek Time for SSTF = %d\n\n", total_seek);
}

int main() {
    int n, initial_head;

    printf("Enter number of requests: ");
    scanf("%d", &n);

    DSA* requests = (DSA*)malloc(n * sizeof(DSA));
    if (!requests) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    printf("Enter request details (request_id arrival_time_stamp cylinder address process_id):\n");
    for (int i = 0; i < n; i++) {
        scanf("%d %d %d %d %d", &requests[i].request_id, &requests[i].arrival_time_stamp,
              &requests[i].cylinder, &requests[i].address, &requests[i].process_id);
    }

    printf("Enter initial head position: ");
    scanf("%d", &initial_head);

    // Sort requests by arrival timestamp for FCFS
    qsort(requests, n, sizeof(DSA), compareArrival);

    FCFS(requests, n, initial_head);
    SSTF(requests, n, initial_head);

    free(requests);
    return 0;
}

/*

Example Input:
5
1 0 55 1024 2
2 2 58 2048 1
3 3 39 4096 3
4 4 18 1024 2
5 5 90 512 1
50

Corresponding Output:

Enter number of requests: 5
Enter request details (request_id arrival_time_stamp cylinder address process_id):
1 0 55 1024 2
2 2 58 2048 1
3 3 39 4096 3
4 4 18 1024 2
5 5 90 512 1
Enter initial head position: 50
FCFS Disk Scheduling:
Servicing request 1 (Process 2) at cylinder 55: head moves from 50 to 55, seek = 5
Servicing request 2 (Process 1) at cylinder 58: head moves from 55 to 58, seek = 3
Servicing request 3 (Process 3) at cylinder 39: head moves from 58 to 39, seek = 19
Servicing request 4 (Process 2) at cylinder 18: head moves from 39 to 18, seek = 21
Servicing request 5 (Process 1) at cylinder 90: head moves from 18 to 90, seek = 72
Total Seek Time for FCFS = 120

SSTF Disk Scheduling:
Servicing request 1 (Process 2) at cylinder 55: head moves from 50 to 55, seek = 5
Servicing request 2 (Process 1) at cylinder 58: head moves from 55 to 58, seek = 3
Servicing request 5 (Process 1) at cylinder 90: head moves from 58 to 90, seek = 32
Servicing request 3 (Process 3) at cylinder 39: head moves from 90 to 39, seek = 51
Servicing request 4 (Process 2) at cylinder 18: head moves from 39 to 18, seek = 21
Total Seek Time for SSTF = 112

*/

