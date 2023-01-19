#include <stdio.h>
#include <unistd.h>

struct Block{
    int isAvailable;
    size_t size;
    struct Block_t* prev;
    struct Block_t* next;
};

typedef struct Block Block_t;



