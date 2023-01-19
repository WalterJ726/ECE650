#include "my_malloc.h"



void* heap_start_addr;
Block_t* BlockHead;
Block_t* BlockTail;

int isInitialized;


void init_malloc(){
	// init heap address
	heap_start_addr = sbrk(0);
	// init manage bi-LinkedList
    BlockHead = NULL;
    BlockTail = NULL;
    isInitialized = 1;
}



void* ff_malloc(size_t size){
	if (!isInitialized){
		// initialize
		init_malloc();
	}
    
    void* ff_block_addr;
    if (BlockHead = NULL){
        void* initial_block_start_addr = sbrk(0);
        void* initial_block_end_addr = sbrk(size + sizeof(Block_t));
        BlockHead = (Block_t*)initial_block_start_addr;

        BlockHead->isAvailable = 0;
        BlockHead->size = size + sizeof(Block_t);
        BlockHead->prev = NULL;
        BlockHead->next = NULL;
        BlockTail = BlockHead;
        ff_block_addr = initial_block_start_addr;
    } else{
        Block_t** curr = &BlockHead;
        while (*curr != NULL){
            if (!(*curr)->isAvailable){
                curr = &(*curr)->next;
            } else{
                if ((*curr)->size >= size + sizeof(Block_t)){
                    ff_block_addr = (*curr);
                    (*curr)->isAvailable = 0;
                    break;
                }else{
                    curr = &(*curr)->next;
                }
            }
        }
        if (*curr == NULL){
            // can not find the available block
            // create a new block
            void* new_block_start_addr = sbrk(0);
            void* new_block_end_addr = sbrk(size + sizeof(Block_t));
            Block_t* current_Block = (Block_t*)new_block_start_addr;
            current_Block->isAvailable = 0;
            current_Block->next = NULL;
            current_Block->size = size + sizeof(Block_t);
            current_Block->prev = BlockTail;
            current_Block->prev->next = current_Block;
            BlockTail = current_Block;
            ff_block_addr = new_block_start_addr;
        }
    }
        return ff_block_addr + sizeof(Block_t);  // first part is the block servered for saving info.
  }


void ff_free(void* ptr){
    if (ptr == NULL){
        return;
    }
    Block_t* free_block_addr = (Block_t*)ptr - sizeof(Block_t);
    free_block_addr->isAvailable = 1;
    return;
}

	
