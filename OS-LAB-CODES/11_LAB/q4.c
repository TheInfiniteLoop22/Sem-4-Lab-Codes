/*
4. A file system uses contiguous allocation of disk space to files. A few blocks on the disk are reserved as spare blocks. If some disk blocks is found to be bad, the file system allocates a spare disk block to it and notes the address of the bad block and its allocated spare block in a “bad blocks table”. This table is consulted while accessing the disk block. Simulate the same.
*/

#include <stdio.h>
#include <stdlib.h>

#define MAX_BLOCKS 100
#define MAX_SPARE 10
#define MAX_FILES 10

typedef struct {
    int bad_block;
    int spare_block;
} BadBlockEntry;

typedef struct {
    int start_block;    // starting block of file allocation
    int length;         // number of contiguous blocks allocated to file
} FileAllocation;

BadBlockEntry bad_blocks_table[MAX_SPARE];
int bad_blocks_count = 0;

int spare_blocks[MAX_SPARE];
int spare_blocks_count = 0;

int findSpareForBadBlock(int block) {
    for (int i = 0; i < bad_blocks_count; i++) {
        if (bad_blocks_table[i].bad_block == block)
            return bad_blocks_table[i].spare_block;
    }
    return -1; // no spare block assigned
}

void addBadBlock(int bad_block) {
    if (bad_blocks_count >= MAX_SPARE) {
        printf("No more spare blocks available.\n");
        return;
    }
    if (spare_blocks_count <= 0) {
        printf("No spare blocks left to assign.\n");
        return;
    }
    int spare = spare_blocks[--spare_blocks_count];
    bad_blocks_table[bad_blocks_count].bad_block = bad_block;
    bad_blocks_table[bad_blocks_count].spare_block = spare;
    bad_blocks_count++;
    printf("Assigned spare block %d for bad block %d\n", spare, bad_block);
}

void accessBlock(int block) {
    int spare = findSpareForBadBlock(block);
    if (spare != -1) {
        printf("Accessing spare block %d instead of bad block %d\n", spare, block);
    } else {
        printf("Accessing block %d\n", block);
    }
}

int main() {
    int total_blocks, num_spare, num_files;

    printf("Enter total number of blocks on disk: ");
    scanf("%d", &total_blocks);
    if (total_blocks > MAX_BLOCKS) {
        printf("Max blocks supported is %d. Setting total blocks to %d\n", MAX_BLOCKS, MAX_BLOCKS);
        total_blocks = MAX_BLOCKS;
    }

    printf("Enter number of spare blocks reserved: ");
    scanf("%d", &num_spare);
    if (num_spare > MAX_SPARE) {
        printf("Max spare blocks supported is %d. Setting spare blocks to %d\n", MAX_SPARE, MAX_SPARE);
        num_spare = MAX_SPARE;
    }
    if (num_spare >= total_blocks) {
        printf("Spare blocks must be less than total blocks. Adjusting spare blocks.\n");
        num_spare = total_blocks - 1;
    }
    spare_blocks_count = num_spare;

    // Initialize spare blocks to the last 'num_spare' blocks (highest numbered)
    for (int i = 0; i < num_spare; i++) {
        spare_blocks[i] = total_blocks - 1 - i;
    }

    printf("Enter number of files: ");
    scanf("%d", &num_files);
    if (num_files > MAX_FILES) {
        printf("Max files supported is %d. Setting files to %d\n", MAX_FILES, MAX_FILES);
        num_files = MAX_FILES;
    }

    FileAllocation files[MAX_FILES];

    for (int i = 0; i < num_files; i++) {
        printf("Enter start block and length for file %d: ", i);
        scanf("%d %d", &files[i].start_block, &files[i].length);
        if (files[i].start_block < 0 || files[i].start_block + files[i].length > total_blocks - num_spare) {
            printf("Invalid allocation for file %d. Exiting.\n", i);
            return 1;
        }
    }

    int num_bad;
    printf("Enter number of bad blocks detected: ");
    scanf("%d", &num_bad);
    for (int i = 0; i < num_bad; i++) {
        int bad_block;
        printf("Enter bad block number %d: ", i+1);
        scanf("%d", &bad_block);
        if (bad_block < 0 || bad_block >= total_blocks) {
            printf("Invalid block number.\n");
            i--;
            continue;
        }
        addBadBlock(bad_block);
    }

    // Access files and blocks
    for (int i = 0; i < num_files; i++) {
        printf("\nAccessing blocks of file %d:\n", i);
        for (int b = files[i].start_block; b < files[i].start_block + files[i].length; b++) {
            accessBlock(b);
        }
    }

    return 0;
}


/*
Enter total number of blocks on disk: 100
Enter number of spare blocks reserved: 5
Enter number of files: 2
Enter start block and length for file 0: 10 5
Enter start block and length for file 1: 20 7
Enter number of bad blocks detected: 2
Enter bad block number 1: 12
Assigned spare block 99 for bad block 12
Enter bad block number 2: 22
Assigned spare block 98 for bad block 22

Accessing blocks of file 0:
Accessing block 10
Accessing block 11
Accessing spare block 99 instead of bad block 12
Accessing block 13
Accessing block 14

Accessing blocks of file 1:
Accessing block 20
Accessing block 21
Accessing spare block 98 instead of bad block 22
Accessing block 23
Accessing block 24
Accessing block 25
Accessing block 26


*/
