#include <stdio.h>
#include <stdlib.h>

typedef struct mab {
    int offset;
    int size;
    int allocated;
    struct mab *next;
    struct mab *prev;
} Mab;

typedef Mab *MabPtr;

enum Policy { FIRST_FIT = 1, NEXT_FIT, BEST_FIT, WORST_FIT };

// Global pointer for Next Fit to remember the last allocated block
MabPtr last_alloc = NULL;

// Creates a new memory block
MabPtr createBlock(int offset, int size, int allocated) {
    MabPtr m = (MabPtr)malloc(sizeof(Mab));
    m->offset = offset;
    m->size = size;
    m->allocated = allocated;
    m->next = NULL;
    m->prev = NULL;
    return m;
}

// Check memory availability based on policy
MabPtr memChk(MabPtr m, int size, int policy) {
    MabPtr candidate = NULL;
    MabPtr iter = m;

    switch (policy) {
        case FIRST_FIT:
            while (iter) {
                if (!iter->allocated && iter->size >= size)
                    return iter;
                iter = iter->next;
            }
            break;

        case NEXT_FIT: {
            MabPtr start = last_alloc ? last_alloc->next : m;
            MabPtr ptr = start;
            while (ptr) {
                if (!ptr->allocated && ptr->size >= size) {
                    last_alloc = ptr;
                    return ptr;
                }
                ptr = ptr->next;
            }
            // Wrap around
            ptr = m;
            while (ptr != start) {
                if (!ptr->allocated && ptr->size >= size) {
                    last_alloc = ptr;
                    return ptr;
                }
                ptr = ptr->next;
            }
            break;
        }

        case BEST_FIT:
            while (iter) {
                if (!iter->allocated && iter->size >= size) {
                    if (!candidate || iter->size < candidate->size)
                        candidate = iter;
                }
                iter = iter->next;
            }
            return candidate;

        case WORST_FIT:
            while (iter) {
                if (!iter->allocated && iter->size >= size) {
                    if (!candidate || iter->size > candidate->size)
                        candidate = iter;
                }
                iter = iter->next;
            }
            return candidate;
    }
    return NULL;
}

// Splits a block if it's larger than needed
MabPtr memSplit(MabPtr m, int size) {
    if (m == NULL || size <= 0 || size > m->size)
        return NULL;

    if (m->size == size) {
        m->allocated = 1;
        return m;
    }
    MabPtr new_blk = createBlock(m->offset + size, m->size - size, 0);
    new_blk->next = m->next;
    new_blk->prev = m;
    if (m->next) m->next->prev = new_blk;
    m->next = new_blk;
    m->size = size;
    m->allocated = 1;
    return m;
}

// Allocates memory based on requested size and policy
MabPtr memAlloc(MabPtr m, int size, int policy) {
    MabPtr candidate = memChk(m, size, policy);
    if (candidate == NULL) {
        printf("Allocation failed: No suitable block for %d bytes\n", size);
        return NULL;
    }
    if (policy == NEXT_FIT) last_alloc = candidate;
    return memSplit(candidate, size);
}

// Merges adjacent free blocks
MabPtr memMerge(MabPtr m) {
    if (!m) return NULL;
    // Merge forward
    while (m->next && !m->allocated && !m->next->allocated) {
        MabPtr n = m->next;
        m->size += n->size;
        m->next = n->next;
        if (n->next) n->next->prev = m;
        free(n);
    }
    // Merge backward
    while (m->prev && !m->allocated && !m->prev->allocated) {
        MabPtr p = m->prev;
        p->size += m->size;
        p->next = m->next;
        if (m->next) m->next->prev = p;
        free(m);
        m = p;
    }
    return m;
}

// Free a memory block and attempt merging
MabPtr memFree(MabPtr m) {
    if (m == NULL) return NULL;
    m->allocated = 0;
    return memMerge(m);
}

// Print memory list for debug/info
void printMemory(MabPtr head) {
    printf("Memory Blocks:\n");
    MabPtr curr = head;
    while (curr) {
        printf("Offset: %4d | Size: %4d | Allocated: %d\n",
               curr->offset, curr->size, curr->allocated);
        curr = curr->next;
    }
    printf("\n");
}

// Free the full list at end
void freeList(MabPtr head) {
    MabPtr curr = head;
    while (curr) {
        MabPtr next = curr->next;
        free(curr);
        curr = next;
    }
}

int main() {
    MabPtr head = createBlock(0, 1024, 0);
    MabPtr blocks[10] = {NULL}; // Track allocations for test/free
    int policy, size, choice, idx = 0;

    printf("Select allocation policy:\n");
    printf("1. First Fit\n2. Next Fit\n3. Best Fit\n4. Worst Fit\n");
    printf("Enter choice: ");
    scanf("%d", &policy);

    while (1) {
        printf("\n1. Allocate\n2. Free block\n3. Print Memory\n4. Exit\nChoice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Enter size to allocate: ");
            scanf("%d", &size);
            blocks[idx] = memAlloc(head, size, policy);
            if (blocks[idx])
                printf("Block allocated at offset %d of size %d (block %d)\n", 
                        blocks[idx]->offset, blocks[idx]->size, idx);
            idx++;
        }
        else if (choice == 2) {
            int b;
            printf("Enter block number to free (from previous allocation): ");
            scanf("%d", &b);
            if (b >= 0 && b < idx && blocks[b] != NULL) {
                memFree(blocks[b]);
                printf("Block %d freed.\n", b);
            } else {
                printf("Invalid block.\n");
            }
        }
        else if (choice == 3) {
            printMemory(head);
        }
        else if (choice == 4) {
            break;
        }
        else {
            printf("Invalid choice!\n");
        }
    }

    freeList(head);
    return 0;
}

