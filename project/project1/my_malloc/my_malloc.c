#include "my_malloc.h"



void* heap_start_addr;
Block_t* BlockHead;
Block_t* BlockTail;

int isInitialized;


// void init_malloc(){
// 	// init heap address
// 	heap_start_addr = sbrk(0);
// 	// init manage bi-LinkedList
//     BlockHead = NULL;
//     BlockTail = NULL;
//     isInitialized = 1;
// }


void removeNodeFromLinkedList(void* _deleteNode){
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
        return;
    } else{
        deleteNode->prev->next = deleteNode->next;
        deleteNode->next->prev = deleteNode->prev;
        return;
    }
}


void addNodeToLinkedList(Block_t* addNode){
    Block_t* curr = BlockHead;
    while(curr != NULL){
        if (curr > addNode){
            break;
        }
        curr = curr->next;
    }

    if (curr == BlockHead){
        addNode->next = BlockHead;
        addNode->prev = NULL;
        addNode->next->prev = addNode;
        BlockHead = addNode;
        return;
    } else if (curr == NULL){
        addNode->next = NULL;
        addNode->prev = BlockTail;
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
    void* new_spilt_block_start_addr = (char*)spiltNode + sizeof(Block_t) + spiltNode->size;
    return new_spilt_block_start_addr;
}


void* ff_malloc(size_t size){
	// if (!isInitialized){
	// 	// initialize
	// 	init_malloc();
	// }
    
    // find block in freed list
    Block_t* curr = BlockHead;
    while (curr != NULL){
        if (curr->size >= size){
            break;
        }
        curr = curr->next;
    }
    void* newblock_start_addr;
    if (curr = NULL){
        // can not find the available block 
        // create a new block
        newblock_start_addr = sbrk(size + sizeof(Block_t));
        Block_t* temp_block_start_addr = (Block_t*)newblock_start_addr;
        temp_block_start_addr->size = size;  
        return (char*)newblock_start_addr + sizeof(Block_t); // first part is the block servered for saving info
    } else if(curr->size < size + sizeof(Block_t)){
        // remove the node in freed list
        newblock_start_addr = curr;
        curr->next = NULL;
        curr->prev = NULL;
        removeNodeFromLinkedList(curr);
        return (char*)newblock_start_addr + sizeof(Block_t);
    } else if (curr->size >= size + sizeof(Block_t)){
        // if size was larger than requested size
        // we should spilt it
        newblock_start_addr = spiltNodeFromLinkedList(curr, size);
        return (char*)newblock_start_addr + sizeof(Block_t);       
    }
  }


void coalesceBlock(void* curr){
    // find left
    // while(1)

    // find right
}

void ff_free(void* ptr){
    if (ptr == NULL){
        return;
    }
    Block_t* free_block_addr = (Block_t*)((char*)ptr - sizeof(Block_t));
    addNodeToLinkedList(free_block_addr);
    // coalesceBlock(free_block_addr);
    return;
}

	
