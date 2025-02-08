#include<stdio.h>

int gcd_consecutive_integers(int a, int b) {
    int smaller = a < b ? a : b;

    for (int i = smaller; i >= 1; i--) {
        if (a % i == 0 && b % i == 0) {
            return i;
        }
    }
    return 1;
}

int main(){
    int a, b;
    
    printf("Enter the first number: ");
    scanf("%d", &a);
    
    printf("Enter the second number: ");
    scanf("%d", &b);
    
    // Find and display the GCD
    int gcd = gcd_consecutive_integers(a, b);
    printf("The GCD of %d and %d is %d.\n", a, b, gcd);
    
    return 0;
}