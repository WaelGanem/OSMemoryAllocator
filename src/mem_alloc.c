#include "mem_alloc.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#include "mem_alloc_types.h"
#include "my_mmap.h"

/* pointer to the beginning of the memory region to manage */
void *heap_begin;

/* Pointer to the first free block in the heap */
memblk_free_t *first_free; 


#define ULONG(x)((long unsigned int)(x))

#if defined(FIRST_FIT)

/* TODO: code specific to the FIRST FIT allocation policy can be
 * inserted here */

/* You can define here functions that will be compiled only if the
 * symbol FIRST_FIT is defined, that is, only if the selected policy
 * is FF */


#elif defined(BEST_FIT)

/* TODO: code specific to the BEST FIT allocation policy can be
 * inserted here */

#elif defined(NEXT_FIT)

/* TODO: code specific to the NEXT FIT allocation policy can be
 * inserted here */

#endif


void run_at_exit(void)
{
    fprintf(stderr,"YEAH B-)\n");
    
    /* TODO: insert your code here */
}




void memory_init(void)
{
    /* register the function that will be called when the programs exits */
    atexit(run_at_exit);

    heap_begin = my_mmap(MEM_POOL_SIZE);
    first_free = (memblk_free_t *)heap_begin;
    first_free->size = MEM_POOL_SIZE;
    first_free->next = NULL;

}

void *memory_alloc(size_t size)
{

    /* TODO : don't forget to call the function print_alloc_info()
     * appropriately */
    header = align_up(sizeof(memblk_allocated_t), MEM_ALIGNMENT);
    payload = align_up(size, MEM_ALIGNMENT);
    real_size = header + payload;
    
    memblk_free_t *current = first_free;//it's a pointer (address)
    memblk_free_t *previous = NULL;

    while(current != NULL && current->size < real_size){
        previous = current;
        current = current->next;
    }
    if(current == NULL){
        print_alloc_error(size); 
        exit(0);
    }
    size_t leftover = current->size - real_size;
    size_t minimal_tail = align_up(sizeof(memblk_free_t), MEM_ALIGNMENT) + MEM_ALIGNMENT;

    if(leftover < minimal_tail){
        if(previous == NULL){
            first_free = current->next;
        } else {
            previous->next = current->next;
        }
        memblk_allocated_t *allocated = (memblk_allocated_t *) current;
        allocated->size = current->size;

        void *payload_pointer = (void *)( (char *) current + header);
        print_alloc_info(payload_pointer, (int)size);
        return payload_pointer;

    } else {
        memblk_free_t *next_free = current->next;
        memblk_allocated_t *allocated = (memblk_allocated_t *) current;
        allocated->size = real_size;

        memblk_free_t *tail_adr = (memblk_free_t *)((char *)current + real_size);
        tail_adr->size = leftover;
        tail_adr->next = next_free;

        if(previous == NULL){
            first_free = tail_adr;
        } else {
            previous->next = tail_adr;
        }
        void *payload_pointer = (void *)((char *)current + header);
        print_alloc_info(payload_pointer, (int)size);
        return payload_pointer;

    }
}

void memory_free(void *p)
{

    /* TODO: insert your code here */

    /* TODO : don't forget to call the function print_free_info()
     * appropriately */

}

size_t memory_get_allocated_block_size(void *addr)
{

    /* TODO: insert your code here */

    return 0;
}


void print_mem_state(void)
{
    /* TODO: insert your code here */
}


void print_info(void) {
    fprintf(stderr, "Memory : [%lu %lu] (%lu bytes)\n", (long unsigned int) heap_begin, (long unsigned int) ((char*)heap_begin+MEM_POOL_SIZE), (long unsigned int) (MEM_POOL_SIZE));
}

void print_free_info(void *addr){
    if(addr){
        fprintf(stderr, "FREE  at : %lu \n", ULONG((char*)addr - (char*)heap_begin));
    }
    else{
        fprintf(stderr, "FREE  at : %lu \n", ULONG(0));
    }
    
}

void print_alloc_info(void *addr, int requested_size){
  if(addr){
    fprintf(stderr, "ALLOC at : %lu (%d byte(s))\n", 
	    ULONG((char*)addr - (char*)heap_begin), requested_size);
  }
  else{
    fprintf(stderr, "Warning, system is out of memory\n"); 
  }
}

void print_alloc_error(int requested_size) 
{
    fprintf(stderr, "ALLOC error : can't allocate %d bytes\n", requested_size);
}


#ifdef MAIN
int main(int argc, char **argv){

  /* The main can be changed, it is *not* involved in tests */
  memory_init();
  print_info();
  int i ; 
  for( i = 0; i < 10; i++){
    char *b = memory_alloc(rand()%8);
    memory_free(b);
  }

  char * a = memory_alloc(15);
  memory_free(a);


  a = memory_alloc(10);
  memory_free(a);

  fprintf(stderr,"%lu\n",(long unsigned int) (memory_alloc(9)));
  return EXIT_SUCCESS;
}
#endif 
