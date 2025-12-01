/*
C program to simulate the LRU approximation (Second Chance) page replacement algorithm. The program will prompt for a frame size, a reference string (sequence of page numbers), simulate the replacement, and then report the total page faults and hit ratio at the end.
C Program: Second Chance (LRU Approximation) Page Replacement
*/
#include <stdio.h>

#define MAX_REF_LEN 100
#define MAX_FRAMES  10

typedef struct {
    int page;
    int ref_bit;
} Frame;

int search(Frame frames[], int nframes, int page) {
    for (int i = 0; i < nframes; i++) {
        if (frames[i].page == page)
            return i;
    }
    return -1;
}

int main() {
    int nframes, nrefs;
    int ref_str[MAX_REF_LEN];

    printf("Enter number of frames (max %d): ", MAX_FRAMES);
    scanf("%d", &nframes);

    printf("Enter length of reference string (max %d): ", MAX_REF_LEN);
    scanf("%d", &nrefs);

    printf("Enter reference string:\n");
    for (int i = 0; i < nrefs; i++)
        scanf("%d", &ref_str[i]);

    Frame frames[MAX_FRAMES];
    for (int i = 0; i < nframes; i++) {
        frames[i].page = -1;
        frames[i].ref_bit = 0;
    }

    int hand = 0;
    int page_faults = 0, hits = 0;

    for (int i = 0; i < nrefs; i++) {
        int page = ref_str[i];
        int pos = search(frames, nframes, page); // page already in frame?

        if (pos != -1) {
            // Hit
            hits++;
            frames[pos].ref_bit = 1; // Set reference bit
            printf("Page %d: HIT\n", page);
        } else {
            // Page fault
            page_faults++;
            // Second chance replacement: search for frame with ref_bit 0
            while (1) {
                if (frames[hand].ref_bit == 0) {
                    printf("Page %d: FAULT. Replace frame %d (old page %d)\n", page, hand, frames[hand].page);
                    frames[hand].page = page;
                    frames[hand].ref_bit = 1;
                    hand = (hand + 1) % nframes;
                    break;
                } else {
                    // Give second chance
                    frames[hand].ref_bit = 0;
                    hand = (hand + 1) % nframes;
                }
            }
        }
    }

    printf("\nTotal Page Faults: %d\n", page_faults);
    printf("Hit Ratio: %.2f\n", (float)hits / nrefs);

    return 0;
}
/*
How the Program Works

    Frame Structure: Each frame stores the current page and its reference bit (0 or 1).

    Second Chance Algorithm:

        On a hit, set the reference bit to 1.

        On a fault, move the "hand" around circularly:

            If ref bit is 0: replace this frame.

            If ref bit is 1: set it to 0 (give a second chance) and skip to next.

    Page Faults and Hits: Counted and displayed at the end.

    Hit Ratio: Calculated as hits divided by total references.


Usage Example

Input:
Enter number of frames (max 10): 3
Enter length of reference string (max 100): 12
Enter reference string:
1 2 3 2 1 4 5 2 1 2 3 2

Output (sample):
Page 1: FAULT. Replace frame 0 (old page -1)
Page 2: FAULT. Replace frame 1 (old page -1)
Page 3: FAULT. Replace frame 2 (old page -1)
Page 2: HIT
Page 1: HIT
Page 4: FAULT. Replace frame 0 (old page 1)
Page 5: FAULT. Replace frame 1 (old page 2)
...
Total Page Faults: 8
Hit Ratio: 0.33
*/
