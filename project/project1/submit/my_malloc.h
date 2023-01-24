#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <limits.h>

struct Block{
    size_t size;
    struct Block* prev;
    struct Block* next;
};

typedef struct Block Block_t;

void* ff_malloc(size_t size);
void* bf_malloc(size_t size);
void ff_free(void* ptr);
void bf_free(void* ptr);
unsigned long get_data_segment_size();
unsigned long get_data_segment_free_space_size();
void printLinkedList();




