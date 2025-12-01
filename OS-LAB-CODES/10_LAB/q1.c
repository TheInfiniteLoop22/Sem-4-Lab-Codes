/*
This system demonstrates how to implement key operations to support memory allocation and deallocation policies such as Best Fit, First Fit, Next Fit, or Worst Fit by modifying the memAlloc function accordingly.
*/



#include <stdio.h>
#include <stdlib.h>

struct mab {
    int offset;          // Start offset of memory block
    int size;            // Size of memory block in bytes
    int allocated;       // 0 if free, 1 if allocated
    struct mab *next;    // Next block in list
    struct mab *prev;    // Previous block in list
};

typedef struct mab Mab;
typedef Mab *MabPtr;

// Find first free block with enough size (First Fit)
MabPtr memChk(MabPtr m, int size) {
    while (m != NULL) {
        if (!m->allocated && m->size >= size)
            return m;
        m = m->next;
    }
    return NULL; // No suitable block found
}

// Split block into allocated part and leftover free part if needed
MabPtr memSplit(MabPtr m, int size) {
    if (m == NULL || size <= 0 || size > m->size)
        return NULL;

    if (m->size == size) {
        m->allocated = 1; // Perfect fit, mark allocated
        return m;
    }

    // Create new free block after allocated block
    MabPtr new_block = (MabPtr)malloc(sizeof(Mab));
    if (new_block == NULL) return NULL; // Allocation failure

    new_block->offset = m->offset + size;
    new_block->size = m->size - size;
    new_block->allocated = 0;
    new_block->next = m->next;
    new_block->prev = m;

    if (m->next != NULL)
        m->next->prev = new_block;

    m->next = new_block;
    m->size = size;
    m->allocated = 1; // Mark allocated

    return m;
}

// Merge adjacent free blocks forward and backward
MabPtr memMerge(MabPtr m) {
    if (m == NULL) return NULL;

    // Merge forward
    while (m->next != NULL && !m->allocated && !m->next->allocated) {
        MabPtr next_block = m->next;
        m->size += next_block->size;
        m->next = next_block->next;
        if (next_block->next != NULL)
            next_block->next->prev = m;
        free(next_block);
    }

    // Merge backward
    while (m->prev != NULL && !m->allocated && !m->prev->allocated) {
        MabPtr prev_block = m->prev;
        prev_block->size += m->size;
        prev_block->next = m->next;
        if (m->next != NULL)
            m->next->prev = prev_block;
        free(m);
        m = prev_block;
    }

    return m;
}

// Allocate a memory block using First Fit
MabPtr memAlloc(MabPtr m, int size) {
    MabPtr candidate = memChk(m, size);
    if (candidate == NULL) {
        printf("Allocation of %d bytes failed: no suitable block\n", size);
        return NULL;
    }

    candidate = memSplit(candidate, size);
    return candidate;
}

// Free a memory block and merge adjacent free blocks
MabPtr memFree(MabPtr m) {
    if (m == NULL) return NULL;

    m->allocated = 0;
    return memMerge(m);
}

// Print all memory blocks for debugging
void printMemoryList(MabPtr head) {
    printf("Memory Blocks:\n");
    MabPtr curr = head;
    while (curr != NULL) {
        printf("Offset: %4d | Size: %4d | Allocated: %d\n",
            curr->offset, curr->size, curr->allocated);
        curr = curr->next;
    }
    printf("\n");
}

// Free entire linked list at program end
void freeMemoryList(MabPtr head) {
    MabPtr curr = head;
    while (curr != NULL) {
        MabPtr next = curr->next;
        free(curr);
        curr = next;
    }
}

int main() {
    // Initialize memory list with one big free block of 1024 bytes
    MabPtr head = (MabPtr)malloc(sizeof(Mab));
    if (head == NULL) {
        printf("Failed to initialize memory\n");
        return -1;
    }

    head->offset = 0;
    head->size = 1024;
    head->allocated = 0;
    head->next = NULL;
    head->prev = NULL;

    printMemoryList(head);

    // Allocate 100 bytes
    MabPtr block1 = memAlloc(head, 100);
    if (block1) {
        printf("Allocated 100 bytes at offset %d\n", block1->offset);
        printMemoryList(head);
    }

    // Allocate 200 bytes
    MabPtr block2 = memAlloc(head, 200);
    if (block2) {
        printf("Allocated 200 bytes at offset %d\n", block2->offset);
        printMemoryList(head);
    }

    // Free first block
    memFree(block1);
    printf("Freed 100-byte block\n");
    printMemoryList(head);

    // Allocate 50 bytes (should fit into freed 100-byte block)
    MabPtr block3 = memAlloc(head, 50);
    if (block3) {
        printf("Allocated 50 bytes at offset %d\n", block3->offset);
        printMemoryList(head);
    }

    // Free second block
    memFree(block2);
    printf("Freed 200-byte block\n");
    printMemoryList(head);

    // Free third block
    memFree(block3);
    printf("Freed 50-byte block\n");
    printMemoryList(head);

    // Clean up allocated list nodes
    freeMemoryList(head);

    return 0;
}
/*
Explanation

    Starts with one free block of 1024 bytes.

    Allocates 100 and 200 bytes using First Fit.

    Frees blocks to demonstrate merging.

    Allocates 50 bytes inside a freed block to show reuse.

    Maintains a doubly linked list representing memory blocks.

    Properly splits and merges blocks to manage fragmentation.

This program can be easily extended to implement Best Fit, Worst Fit, or Next Fit policies by modifying memChk or adding pointers for Next Fit state.
*/
