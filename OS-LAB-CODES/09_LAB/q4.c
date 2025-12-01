#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *array;
    int size;
    int sum;
} SumData;

void* sum_even(void* arg) {
    SumData *data = (SumData*)arg;
    data->sum = 0;
    for (int i = 0; i < data->size; i++)
        if (data->array[i] % 2 == 0)
            data->sum += data->array[i];
    return NULL;
}

void* sum_odd(void* arg) {
    SumData *data = (SumData*)arg;
    data->sum = 0;
    for (int i = 0; i < data->size; i++)
        if (data->array[i] % 2 != 0)
            data->sum += data->array[i];
    return NULL;
}

int main() {
    int n;
    printf("Enter the size of the array: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid array size.\n");
        return 1;
    }

    int *array = malloc(n * sizeof(int));
    if (!array) {
        perror("Memory allocation failed");
        return 1;
    }

    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++)
        if (scanf("%d", &array[i]) != 1) {
            printf("Invalid input.\n");
            free(array);
            return 1;
        }

    SumData evenData = {array, n, 0};
    SumData oddData = {array, n, 0};
    pthread_t evenThread, oddThread;

    pthread_create(&evenThread, NULL, sum_even, &evenData);
    pthread_create(&oddThread, NULL, sum_odd, &oddData);

    pthread_join(evenThread, NULL);
    pthread_join(oddThread, NULL);

    printf("Sum of even numbers = %d\n", evenData.sum);
    printf("Sum of odd numbers = %d\n", oddData.sum);

    free(array);
    return 0;
}

