#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Shared data structure to hold Fibonacci numbers and count
typedef struct {
    int count;      // Number of Fibonacci numbers to generate
    int *array;     // Pointer to dynamically allocated array
} FibData;

// Thread function to generate Fibonacci sequence
void* generate_fibonacci(void* param) {
    FibData *fibData = (FibData*) param;
    int n = fibData->count;
    int *fib = fibData->array;

    // Base cases
    if (n > 0) fib[0] = 0;
    if (n > 1) fib[1] = 1;

    // Compute Fibonacci sequence
    for (int i = 2; i < n; i++) {
        fib[i] = fib[i-1] + fib[i-2];
    }

    pthread_exit(NULL);  // Terminate thread
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number_of_fibonacci_terms>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        printf("Please enter a positive integer for Fibonacci terms.\n");
        return 1;
    }

    // Allocate memory for Fibonacci array
    int *fibArray = malloc(n * sizeof(int));
    if (fibArray == NULL) {
        perror("Failed to allocate memory");
        return 1;
    }

    // Prepare data to pass to thread
    FibData data = { n, fibArray };

    pthread_t fibThread;

    // Create thread to generate Fibonacci sequence
    if (pthread_create(&fibThread, NULL, generate_fibonacci, &data) != 0) {
        perror("Failed to create thread");
        free(fibArray);
        return 1;
    }

    // Wait for the child thread to finish
    pthread_join(fibThread, NULL);

    // Print the generated Fibonacci sequence
    printf("Fibonacci sequence of %d terms:\n", n);
    for (int i = 0; i < n; i++) {
        printf("%d ", fibArray[i]);
    }
    printf("\n");

    free(fibArray);
    return 0;
}

