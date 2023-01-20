#include <stdio.h>
#include <unistd.h>
#include <assert.h>

struct Block{
    size_t size;
    struct Block* prev;
    struct Block* next;
};

typedef struct Block Block_t;

void* ff_malloc(size_t size);
void ff_free(void* ptr);
// void printLinkedList();




