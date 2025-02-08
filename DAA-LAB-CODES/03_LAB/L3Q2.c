#include <stdio.h>
#include <string.h>

// Function to implement brute-force string matching
void bruteForceStringMatching(const char text[], const char pattern[]) {
    int textLength = strlen(text);     // Length of the text
    int patternLength = strlen(pattern); // Length of the pattern

    int found = 0; // Flag to check if the pattern is found

    printf("Pattern found at indices: ");

    // Iterate through the text
    for (int i = 0; i <= textLength - patternLength; i++) {
        int j;

        // Compare pattern with the substring in the text
        for (j = 0; j < patternLength; j++) {
            if (text[i + j] != pattern[j]) {
                break;
            }
        }

        // If pattern matches, print the starting index
        if (j == patternLength) {
            printf("%d ", i);
            found = 1;
        }
    }

    if (!found) {
        printf("None");
    }
    printf("\n");
}

int main() {
    char text[1000], pattern[100];

    // Input the text
    printf("Enter the text: ");
    scanf("%[^\n]%*c", text); // Read until newline and discard it

    // Input the pattern
    printf("Enter the pattern: ");
    scanf("%[^\n]%*c", pattern); // Read until newline and discard it

    // Perform brute-force string matching
    bruteForceStringMatching(text, pattern);

    return 0;
}
