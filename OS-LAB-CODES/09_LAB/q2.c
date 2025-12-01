#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Structure to hold input data
typedef struct {
    int n;  // Number of non-negative integers to sum (0 to n)
} SumData;

// Thread function to compute sum of integers from 0 to n
void* calculate_sum(void* arg) {
    SumData* data = (SumData*) arg;  // Cast void* to SumData*
    int n = data->n;

    // Calculate sum of first n non-negative integers
    int sum = 0;
    for (int i = 0; i <= n; i++) {
        sum += i;
    }

    // Return sum as void* by casting integer to pointer
    // (Using intptr_t to safely cast int to pointer type)
    return (void*)(intptr_t)sum;
}

int main() {
    pthread_t thread;
    SumData data;

    // Get number from user
    printf("Enter a non-negative integer: ");
    if (scanf("%d", &data.n) != 1 || data.n < 0) {
        printf("Invalid input. Please enter a non-negative integer.\n");
        return 1;
    }

    // Create a thread to calculate sum
    if (pthread_create(&thread, NULL, calculate_sum, &data) != 0) {
        perror("Failed to create thread");
        return 1;
    }

    // Wait for thread to finish and retrieve the result
    void* thread_result;
    pthread_join(thread, &thread_result);

    // Cast back the result from void* to int
    int sum_result = (int)(intptr_t)thread_result;

    // Print the result
    printf("Summation of integers from 0 to %d is %d\n", data.n, sum_result);

    return 0;
}

