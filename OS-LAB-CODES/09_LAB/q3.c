#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Structure to hold parameters for prime generation thread
typedef struct {
    int start;
    int end;
    int *primes;      // dynamically allocated array to store primes found
    int count;        // number of primes found
} PrimeData;

// Function to check if a number is prime
bool is_prime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

// Thread function to generate primes
void* generate_primes(void* arg) {
    PrimeData *data = (PrimeData*)arg;
    int start = data->start;
    int end = data->end;

    // Allocate an array large enough to hold all numbers in range,
    // we'll store only primes and count them properly later
    int* temp = malloc((end - start + 1) * sizeof(int));
    if (temp == NULL) {
        perror("Malloc failed");
        pthread_exit(NULL);
    }

    int count = 0;
    for (int num = start; num <= end; num++) {
        if (is_prime(num)) {
            temp[count++] = num;
        }
    }

    // Resize the array to the actual count
    data->primes = realloc(temp, count * sizeof(int));
    if (data->primes == NULL && count > 0) {
        // If realloc failed but count > 0, free old allocation and exit
        free(temp);
        perror("Realloc failed");
        pthread_exit(NULL);
    }
    data->count = count;
    pthread_exit(NULL);
}

int main() {
    int start, end;

    // Read the range from user
    printf("Enter start and end numbers (start <= end): ");
    if (scanf("%d %d", &start, &end) != 2 || start > end) {
        printf("Invalid input range.\n");
        return 1;
    }

    PrimeData data;
    data.start = start;
    data.end = end;
    data.primes = NULL;
    data.count = 0;

    pthread_t thread;

    // Create thread to generate prime numbers
    if (pthread_create(&thread, NULL, generate_primes, &data) != 0) {
        perror("Failed to create thread");
        return 1;
    }

    // Wait for prime generation to complete
    pthread_join(thread, NULL);

    // Output the prime numbers
    printf("Prime numbers between %d and %d are:\n", start, end);
    for (int i = 0; i < data.count; i++) {
        printf("%d ", data.primes[i]);
    }
    printf("\n");

    // Free allocated memory
    free(data.primes);

    return 0;
}

