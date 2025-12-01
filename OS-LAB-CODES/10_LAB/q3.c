/*
program implementing a segment table and converting given logical addresses to physical addresses based on the segmented memory layout shown in your provided image.
Explanation:

    The segment table holds base (starting address in physical memory) and limit (segment size) for each segment.

    Logical address consists of a segment number and an offset within that segment.

    To convert a logical address to a physical address:

        Check if the offset is less than the segment limit (valid offset).

        If valid, physical address = base + offset.

        If invalid, report an error (address out of segment bounds).



Segment  |  Base  |  Limit
---------+--------+-------
0        |  1400  |  700  
1        |  2400  |  500  
2        |  3200  |  470  
3        |  3700  |  430  
4        |  4100  |  530  

Required Logical Addresses to Convert:

    (i) 53rd byte of segment 2

    (ii) 852nd byte of segment 3

    (iii) 1222nd byte of segment 0


*/


#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int base;
    int limit;
} Segment;

// Function to convert logical to physical address
int logical_to_physical(Segment segTable[], int segment, int offset) {
    if (segment < 0 || segment >= 5) {
        printf("Error: Invalid segment number %d\n", segment);
        return -1;
    }
    if (offset < 0 || offset >= segTable[segment].limit) {
        printf("Error: Offset %d out of bounds for segment %d (limit %d)\n",
               offset, segment, segTable[segment].limit);
        return -1;
    }
    return segTable[segment].base + offset;
}

int main() {
    // Initialize segment table from given data
    Segment segTable[5] = {
        {1400, 700},  // segment 0
        {2400, 500},  // segment 1
        {3200, 470},  // segment 2
        {3700, 430},  // segment 3
        {4100, 530}   // segment 4
    };
    
    int logical_addresses[][2] = {
        {2, 53},
        {3, 852},
        {0, 1222}
    };

    for (int i = 0; i < 3; i++) {
        int segment = logical_addresses[i][0];
        int offset = logical_addresses[i][1];
        int physical = logical_to_physical(segTable, segment, offset);
        if (physical != -1) {
            printf("Logical address (%d, %d) -> Physical address: %d\n", segment, offset, physical);
        }
    }

    return 0;
}
/*
Explanation for Logical Address Conversions:

    For (i) segment 2 offset 53 → Check offset < 470 (segment 2 limit). 53 < 470 is valid.
    Physical address = 3200 + 53 = 3253.

    For (ii) segment 3 offset 852 → Check offset < 430. 852 ≥ 430 is invalid.
    Prints error (offset out of bounds).

    For (iii) segment 0 offset 1222 → Check offset < 700. 1222 ≥ 700 is invalid.
    Prints error (offset out of bounds).

*/
