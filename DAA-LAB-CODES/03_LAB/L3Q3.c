#include <stdio.h>

int main() {
    int n;
    printf("Enter the number of elements: ");
    scanf("%d", &n);
    int set[n];
    printf("Enter the elements: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &set[i]);
    }

    int totalSum = 0;
    for (int i = 0; i < n; i++) {
        totalSum += set[i];
    }

    if (totalSum % 2 != 0) {
        printf("Partition not possible\n");
        return 0;
    }

    int target = totalSum / 2;

    for (int mask = 1; mask < (1 << n); mask++) {
        int subsetSum = 0;
        int subset1[n], idx1 = 0;

        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                subsetSum += set[i];
                subset1[idx1++] = set[i];
            }
        }

        if (subsetSum == target) {
            printf("Partition found\n");
            printf("Subset 1: ");
            for (int i = 0; i < idx1; i++) {
                printf("%d ", subset1[i]);
            }
            printf("\n");

            printf("Subset 2: ");
            int idx2 = 0;
            for (int i = 0; i < n; i++) {
                int found = 0;
                for (int j = 0; j < idx1; j++) {
                    if (set[i] == subset1[j]) {
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    printf("%d ", set[i]);
                }
            }
            printf("\n");
            return 0;
        }
    }

    printf("Partition not possible\n");
    return 0;
}
