// #include "my_malloc.h"



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
    
    void* ff_addr;
    if (BlockHead = NULL){
        void* initial_heap_start_addr = sbrk(size + sizeof(Block_t));
            BlockHead = (Block_t*)initial_heap_start_addr;

            BlockHead->isAvailable = 0;
            BlockHead->size=size;
            BlockHead->prev = NULL;
            BlockHead->next = NULL;
            
            BlockTail = BlockHead;
            ff_addr = initial_heap_start_addr - sizeof(Block_t);
    } else{
        Block_t** curr = &BlockHead;
        while (*curr != NULL){
            if (!(*curr)->isAvailable){
                curr = &(*curr)->next;
            } else{
                if ((*curr)->size >= size + sizeof(Block_t)){
                    ff_addr = (*curr) - sizeof(Block_t);
                    (*curr)->isAvailable = 0;
                    break;
                }
            }
        }
        if (*curr == NULL){
            // can not find the available block
            // create a new block
            void* new_heap_start_addr = sbrk(size + sizeof(Block_t));
            Block_t* current_Block = (Block_t*)new_heap_start_addr;
            current_Block->isAvailable = 0;
            current_Block->next = NULL;
            current_Block->size = size;
            current_Block->prev = BlockTail;
            BlockTail = current_Block;
            ff_addr = new_heap_start_addr - sizeof(Block_t);
        }
    }
        return ff_addr;
  }



// int main(){
// 	printf("first malloc addr: %p\n", ff_malloc(10));
// }


	
