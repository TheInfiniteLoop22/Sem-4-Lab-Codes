// Write a program to sort set of integers using bubble sort. Analyze its time
// efficiency. Obtain the experimental result of order of growth. Plot the result for
// the best and worst case.


#include <stdio.h>

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    int n;

    printf("Enter the number of elements: ");
    scanf("%d", &n);

    int arr[n];

    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    bubbleSort(arr, n);

    printf("Sorted array:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}


















/*
Time Efficiency Analysis:

1. Best Case (Already Sorted):
   - In the best case, no swapping is required.
   - Outer loop runs (n-1) times, and the inner loop comparisons are made for each iteration.
   - Time Complexity: O(n).

2. Worst Case (Reversed Order):
   - The algorithm performs the maximum number of swaps.
   - Outer loop runs (n-1) times, and the inner loop comparisons are made for each iteration.
   - Total number of comparisons: (n-1) + (n-2) + ... + 1 = n(n-1)/2.
   - Time Complexity: O(n^2).

3. Average Case (Random Order):
   - In the average case, the number of swaps and comparisons is proportional to the worst case.
   - Time Complexity: O(n^2).

Bubble sort is simple to implement but inefficient for large datasets compared to more advanced algorithms like QuickSort or MergeSort.
*/
