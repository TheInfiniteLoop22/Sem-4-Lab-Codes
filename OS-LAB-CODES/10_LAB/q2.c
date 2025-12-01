/*
C program implementing a Multilevel Feedback Queue (MLFQ) with three queues, each having different scheduling policies. It uses dynamic memory allocation (malloc) to create processes and manages them in queues.

    Queue 0: Round Robin scheduling with quantum 4.

    Queue 1: Round Robin scheduling with quantum 8.

    Queue 2: First-Come-First-Served (FCFS) scheduling.

The program enqueues some example processes and runs the scheduler, which moves processes between queues based on their remaining execution time
*/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_PROCESSES 100

// Process structure
typedef struct process {
    int pid;
    int burst_time;
    int remaining_time;
    int queue_level; // 0 - highest, 2 - lowest
    struct process *next;
} Process;

// Queue structure
typedef struct queue {
    Process *front;
    Process *rear;
    int quantum;
    int scheduling_policy; // 0 = FCFS, 1 = Round Robin
} Queue;

// Create a new process
Process* create_process(int pid, int burst_time) {
    Process *p = (Process *)malloc(sizeof(Process));
    if (p == NULL) {
        perror("Failed to allocate process");
        exit(1);
    }
    p->pid = pid;
    p->burst_time = burst_time;
    p->remaining_time = burst_time;
    p->queue_level = 0;
    p->next = NULL;
    return p;
}

// Initialize a queue
void init_queue(Queue *q, int quantum, int scheduling_policy) {
    q->front = NULL;
    q->rear = NULL;
    q->quantum = quantum;
    q->scheduling_policy = scheduling_policy;
}

// Enqueue a process
void enqueue(Queue *q, Process *p) {
    p->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = p;
    } else {
        q->rear->next = p;
        q->rear = p;
    }
}

// Dequeue a process
Process* dequeue(Queue *q) {
    if (q->front == NULL) return NULL;
    Process *temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL)
        q->rear = NULL;
    temp->next = NULL;
    return temp;
}

// Round Robin scheduling: run process for min(quantum, remaining_time)
int rr_schedule(Process *p, int quantum) {
    int time_slice = (p->remaining_time < quantum) ? p->remaining_time : quantum;
    printf("Process %d running for %d units; Remaining before run: %d\n",
           p->pid, time_slice, p->remaining_time);
    sleep(1); // simulate running
    p->remaining_time -= time_slice;
    printf("Process %d remaining time after running: %d\n", p->pid, p->remaining_time);
    return p->remaining_time == 0; // returns 1 if finished
}

// FCFS scheduling: run process till completion
int fcfs_schedule(Process *p) {
    printf("Process %d running for %d units till completion\n", p->pid, p->remaining_time);
    sleep(1); // simulate running
    p->remaining_time = 0;
    return 1; // finished
}

// MLFQ Scheduler main loop
void mlfq_scheduler(Queue queues[], int num_queues) {
    while (1) {
        int all_empty = 1;
        for (int i = 0; i < num_queues; i++) {
            if (queues[i].front != NULL) {
                all_empty = 0;
                Process *p = dequeue(&queues[i]);

                int finished = 0;
                if (queues[i].scheduling_policy == 0) { // FCFS
                    finished = fcfs_schedule(p);
                } else if (queues[i].scheduling_policy == 1) { // RR
                    finished = rr_schedule(p, queues[i].quantum);
                }

                if (!finished) {
                    if (i + 1 < num_queues) {
                        p->queue_level = i + 1; // Demote to lower priority queue
                        printf("Process %d demoted to queue %d\n", p->pid, p->queue_level);
                        enqueue(&queues[i + 1], p);
                    } else {
                        // Stay in last queue
                        enqueue(&queues[i], p);
                    }
                } else {
                    printf("Process %d finished execution\n", p->pid);
                    free(p);
                }
                break; // Schedule one process at a time
            }
        }
        if (all_empty) {
            printf("All processes completed. Scheduler exiting.\n");
            break;
        }
    }
}

int main() {
    Queue queues[3];

    // Initialize queues:
    // Queue 0: RR with quantum 4
    // Queue 1: RR with quantum 8
    // Queue 2: FCFS
    init_queue(&queues[0], 4, 1);
    init_queue(&queues[1], 8, 1);
    init_queue(&queues[2], 0, 0);

    // Create and enqueue example processes in highest priority queue
    enqueue(&queues[0], create_process(1, 10));
    enqueue(&queues[0], create_process(2, 6));
    enqueue(&queues[0], create_process(3, 12));

    // Run the MLFQ scheduler
    mlfq_scheduler(queues, 3);

    return 0;
}

