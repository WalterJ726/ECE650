#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <limits.h>
#include <pthread.h>

struct Block{
    size_t size;
    struct Block* prev;
    struct Block* next;
};

typedef struct Block Block_t;


void* ts_malloc_lock(size_t size);
void ts_free_lock(void* ptr);

void* ts_malloc_nolock(size_t size);
void ts_free_nolock(void* ptr);

unsigned long get_data_segment_size();
unsigned long get_data_segment_free_space_size();
void printLinkedList();




