#include <stdio.h>
#define MAX 1024

// Struct to store factorization of numbers
typedef struct {
    int size;
    int factor[MAX + 1];
    int exponent[MAX + 1];
} Factorization;

// Function to find the factorization of a number
void FindFactorization(int num, Factorization* fact) {
    int i, k = 0; // Removed 'j' since it was unused
    int n = num;

    fact->size = 0; // Initialize size to 0

    for (i = 2; i <= n; i++) {
        int c = 0;

        while (n % i == 0) {
            c++;
            n /= i;
        }

        if (c > 0) {
            fact->factor[k] = i;
            fact->exponent[k] = c;
            k++;
        }
    }

    fact->size = k - 1; // Update the size of the factorization
}

int power(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}


// Function to display the factorization
void DisplayFactorization(int num, Factorization fact) {
    int i;
    printf("Prime factors of %d = ", num);

    for (i = 0; i <= fact.size; i++) {
        printf("%d", fact.factor[i]);

        if (fact.exponent[i] > 1)
            printf("^%d", fact.exponent[i]);

        if (i < fact.size)
            printf("*");
        else
            printf("\n");
    }
}

// Function to compute the GCD using the middle school procedure
int gcdMiddleSchoolProcedure(int a, int b) {
    Factorization aFact, bFact;
    int result = 1, i = 0, j = 0;

    // Handle special case when either number is 0
    if (a == 0 || b == 0) {
        return a == 0 ? b : a;
    }

    // Step 1: Find and display the factorization of a
    FindFactorization(a, &aFact);
    DisplayFactorization(a, aFact);

    // Step 2: Find and display the factorization of b
    FindFactorization(b, &bFact);
    DisplayFactorization(b, bFact);

    // Step 3 and 4: Apply the middle school procedure to find the GCD
    while (i <= aFact.size && j <= bFact.size) {
        if (aFact.factor[i] < bFact.factor[j]) {
            i++;
        } else if (bFact.factor[j] < aFact.factor[i]) {
            j++;
        } else { // If factors are equal
            int minExponent = aFact.exponent[i] < bFact.exponent[j] ? aFact.exponent[i] : bFact.exponent[j];
            result *= power(aFact.factor[i], minExponent);
            i++;
            j++;
        }
    }

    return result;
}

// Driver code
int main() {
    int m, n;

    // Take user input for m and n
    printf("Enter two integers (m and n): ");
    scanf("%d %d", &m, &n);

    // Display GCD result
    printf("GCD(%d, %d) = %d\n", m, n, gcdMiddleSchoolProcedure(m, n));
    return 0;
}
