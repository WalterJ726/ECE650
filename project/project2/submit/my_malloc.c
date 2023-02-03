#include "my_malloc.h"

void* heap_start_addr;
Block_t* BlockHead_lock;
Block_t* BlockTail_lock;

__thread Block_t* BlockHead_nolock;
__thread Block_t* BlockTail_nolock;
unsigned long total_segment_size;
unsigned long total_freed_size;
int isInitialized;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void removeNodeFromLinkedList(void* _deleteNode, Block_t* BlockHead, Block_t* BlockTail){
    assert(_deleteNode != NULL);
    Block_t* deleteNode = _deleteNode;
    if (deleteNode == BlockHead){
        if (deleteNode == BlockTail){
            BlockHead = NULL;
            BlockTail = NULL;
            return;
        }
        deleteNode->next->prev = NULL;
        BlockHead = deleteNode->next;
        return;
    } else if (deleteNode == BlockTail){
        BlockTail = deleteNode->prev;
        deleteNode->prev->next = NULL;
        return;
    } else{
        deleteNode->prev->next = deleteNode->next;
        deleteNode->next->prev = deleteNode->prev;
        return;
    }
}


void addNodeToLinkedList(Block_t* addNode, Block_t* BlockHead, Block_t* BlockTail){
    Block_t* curr = BlockHead;
    while(curr != NULL){
        if (curr > addNode){
            break;
        }
        curr = curr->next;
    }

    if (curr == BlockHead){
        if (curr == NULL){
            BlockTail = addNode;
            addNode->next = NULL;
            addNode->prev = NULL;
            BlockHead = addNode;
            return;
        }
        addNode->next = BlockHead;
        addNode->prev = NULL;
        addNode->next->prev = addNode;
        BlockHead = addNode;
        return;
    } else if (curr == NULL){
        addNode->next = NULL;
        addNode->prev = BlockTail;
        addNode->prev->next = addNode;
        BlockTail = addNode;
        return;
    } else{
        addNode->next = curr;
        addNode->prev = curr->prev;
        curr->prev->next = addNode;
        curr->prev = addNode;
        return;
    }
}

void* spiltNodeFromLinkedList(void* _spiltNode, size_t size){
    Block_t* spiltNode = _spiltNode;
    spiltNode->size = spiltNode->size - sizeof(Block_t) - size;
    total_freed_size -= (sizeof(Block_t) + size);
    void* new_spilt_block_start_addr = (void*)spiltNode + sizeof(Block_t) + spiltNode->size;
    Block_t* temp_block_start_addr = (Block_t*)new_spilt_block_start_addr;
    temp_block_start_addr->size = size;
    temp_block_start_addr->next = NULL;
    temp_block_start_addr->prev = NULL;
    return new_spilt_block_start_addr;
}

Block_t* findBlockInFreedList(size_t size, Block_t* BlockHead){
    // best fit
    size_t minSize = INT_MAX;
    Block_t* curr = BlockHead;
    Block_t* minSize_Block = NULL;
    while (curr != NULL){
    if (curr->size >= size && curr->size < minSize){
        minSize_Block = curr;
        minSize = curr->size;
        if (curr->size == size){
            break;
        }
    }
    curr = curr->next;
    }
    return minSize_Block;       
}

void coalesceBlock(Block_t* ptr, Block_t* BlockHead, Block_t* BlockTail){
    // find from curr to tail
    Block_t* curr = ptr;
    if (curr->size + sizeof(Block_t) + (void*)curr == (void*)curr->next){
        curr->size = curr->size + curr->next->size + sizeof(Block_t);
        removeNodeFromLinkedList(curr->next, BlockHead, BlockTail);
    }
    // find from curr back to head
    curr = ptr;
    if (curr->prev != NULL && curr->prev->size + sizeof(Block_t) + (void*)curr->prev == (void*)curr){
        curr->prev->size = curr->prev->size + curr->size + sizeof(Block_t);
        Block_t* temp = curr->prev;
        removeNodeFromLinkedList(curr, BlockHead, BlockTail);
        curr = temp;
        // continue;
    }
}


void* ts_malloc_lock(size_t size){
    // find block in freed list
    pthread_mutex_lock(&lock);
    Block_t* curr = findBlockInFreedList(size, BlockHead_lock);
    void* newblock_start_addr;
    if (curr == NULL){
        // can not find the available block 
        // create a new block
        newblock_start_addr = sbrk(size + sizeof(Block_t));
        if (newblock_start_addr == (void*)-1){
            // sbrk failed
            return NULL;
        }
        total_segment_size += (size + sizeof(Block_t));
        Block_t* temp_block_start_addr = (Block_t*)newblock_start_addr;
        temp_block_start_addr->size = size;
        pthread_mutex_unlock(&lock);
        return (void*)newblock_start_addr + sizeof(Block_t); // first part is the block servered for saving info
    } else if(curr->size < size + sizeof(Block_t)){
        // remove the node in freed list
        newblock_start_addr = curr;
        total_freed_size -= (sizeof(Block_t) + curr->size);
        removeNodeFromLinkedList(curr, BlockHead_lock, BlockTail_lock);
        // curr->size = size;
        curr->next = NULL;
        curr->prev = NULL;
        pthread_mutex_unlock(&lock);
        return (void*)newblock_start_addr + sizeof(Block_t);
    } 
    else if (curr->size >= size + sizeof(Block_t)){
        // if size was larger than requested size
        // we should spilt it
        newblock_start_addr = spiltNodeFromLinkedList(curr, size);
        pthread_mutex_unlock(&lock);
        return (void*)newblock_start_addr + sizeof(Block_t);       
    }
}


void ts_free_lock(void* ptr){
    pthread_mutex_lock(&lock);
    if (ptr == NULL){
        return;
    }
    Block_t* free_block_addr = (Block_t*)((void*)ptr - sizeof(Block_t));
    total_freed_size += (free_block_addr->size + sizeof(Block_t));
    addNodeToLinkedList(free_block_addr, BlockHead_lock, BlockTail_lock);
    coalesceBlock(free_block_addr, BlockHead_lock, BlockTail_lock);
    pthread_mutex_unlock(&lock);
    return;
}

void* ts_malloc_nolock(size_t size){
        // find block in freed list
    Block_t* curr = findBlockInFreedList(size, BlockHead_nolock);
    void* newblock_start_addr;
    if (curr == NULL){
        // can not find the available block 
        // create a new block
        pthread_mutex_lock(&lock);
        newblock_start_addr = sbrk(size + sizeof(Block_t));
        pthread_mutex_unlock(&lock);
        if (newblock_start_addr == (void*)-1){
            // sbrk failed
            return NULL;
        }
        total_segment_size += (size + sizeof(Block_t));
        Block_t* temp_block_start_addr = (Block_t*)newblock_start_addr;
        temp_block_start_addr->size = size;
        return (void*)newblock_start_addr + sizeof(Block_t); // first part is the block servered for saving info
    } else if(curr->size < size + sizeof(Block_t)){
        // remove the node in freed list
        newblock_start_addr = curr;
        total_freed_size -= (sizeof(Block_t) + curr->size);
        removeNodeFromLinkedList(curr, BlockHead_nolock, BlockTail_nolock);
        // curr->size = size;
        curr->next = NULL;
        curr->prev = NULL;
        return (void*)newblock_start_addr + sizeof(Block_t);
    } 
    else if (curr->size >= size + sizeof(Block_t)){
        // if size was larger than requested size
        // we should spilt it
        newblock_start_addr = spiltNodeFromLinkedList(curr, size);
        return (void*)newblock_start_addr + sizeof(Block_t);       
    }

}
void ts_free_nolock(void* ptr){
    if (ptr == NULL){
        return;
    }
    Block_t* free_block_addr = (Block_t*)((void*)ptr - sizeof(Block_t));
    total_freed_size += (free_block_addr->size + sizeof(Block_t));
    addNodeToLinkedList(free_block_addr, BlockHead_nolock, BlockTail_nolock);
    coalesceBlock(free_block_addr, BlockHead_nolock, BlockTail_nolock);
    return;
}

unsigned long get_data_segment_size(){
    return total_segment_size; 
}

unsigned long get_data_segment_free_space_size(){
    return total_freed_size;
}
// void printLinkedList(){
//     // used for debug, print the freed linkedlist
//     Block_t* curr = BlockHead;
//     printf("from Head to Tail:");
//     while (curr != NULL){
//         printf("%p -> ", curr);
//         curr = curr -> next;
//     }
//     curr = BlockTail;
//     printf("\n");
//     // printf("from Tail to Head:");
//     // while (curr != NULL)
//     // {
//     //     printf("%p -> ", curr);
//     //     curr = curr -> prev;
//     // }
//     printf("\n\n\n");
// }

	
