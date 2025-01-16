#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void main (void){
    
    char *ptr;
    // call malloc
    ptr = malloc(0x10); // malloc returns a pointer to the payload section of the chunk
    strcpy ( ptr, "panda");

    // Print the chunk metadata in order 
    // First check if it is free or not
    int is_free = chunk_free(&ptr);
    if ( is_free == 1){
        

    }
    else {

 next_chunk = *(current_chunk_addr + chunk_size);

    }

}
/* Function that tells whether a chunks is free or not 
 * [Chunk A][Chunk B][Chunk C]
           ^       ^
           |       |
         P=1     P=0
 * P = 1 in Chunk B means Chunk A is in use
 * P = 0 in Chunk C means Chunk B is free
 *
 * Hence to check if a chunk is free,
 * Find the next chunk(current_chunk_addr + chunk_size)
 * Look at its P flag
 * If P=0, current chunk is free
 * If P=1, current chunk is in use
 *
 * returns 0 or 1 
 * 
 * */
int chunk_free(char *ptr){

    char *current_chunk_addr = ptr - sizeof(size_t); // Go back 8 bytes
                                                     
    size_t chunk_size = *(size_t *)current_chunk_addr & ~7;
    char *next_chunk = current_chunk_addr + chunk_size;
    size_t next_chunk_size = *(size_t *)next_chunk;
    size_t next_chunk_amP = next_chunk_size & 1;

    if (next_chunk_amP == 1){
        return 1; // current chunk is IN USE
    }

    return 0; // current chunk is FREE

}

/* Function that tells the size of the chunk*/
void get_chunk_size(char *ptr) {

    char *current_chunk_addr = ptr - sizeof(size_t);

    size_t chunk_size = *(size_t *) current_chunk_addr & ~7;

}

/* Function that tells whether the prev chunk is free
 */
void prev_chunk_free(char *ptr){

    char *current_chunk_addr = ptr - sizeof(size_t);

    size_t chunk_size = *(size_t *) current_chunk_addr & ~7;
    char *next_chunk = current_chunk_addr + chunk_size;
    size_t next_chunk_size = *(size_t *) next_chunk & ~7;
    char *prev_chunk = next_chunk + next_chunk_size;
    size_t prev_chunk_size = *(size_t *) prev_chunk;
    size_t prev_chunk_amP = prev_chunk_size & 1;

    if (prev_chunk_amP == 1){
        return 1; // prev chunk is IN USE
    }

    return 0; // current chunk is FREE
}


/* Function that tells whether the chunk is mmapped */
void is_mmapped(char *ptr){

    char *current_chunk_addr = ptr - sizeof(size_t);

    size_t chunk_size = *(size_t *)current_chunk_addr & ~7;
    char *next_chunk = current_chunk_addr + chunk_size;
    size_t next_chunk_sizes = *(size_t *) next_chunk;
    size_t next_chunk_aMp = next_chunk_size & 2;

    if (next_chunk_aMp == 1){
        return 1; // current chunk is MMAP'd
    }

    return 0; // current chunk is not MMAP'd
}


/* Fuction that tells where the chunk comes from */
void non_main_arena(char *ptr){

    char *current_chunk_addr = ptr - sizeof(size_t);
    size_t chunk_size = *(size_t *)current_chunk_addr & ~7;
    char *next_chunk = current_chunk_addr + chunk_size;
    size_t next_chunk_sizes = *(size_t *) next_chunk;
    size_t next_chunk_Amp = next_chunk_size & 4;
    
    if (next_chunk_Amp == 1) {
        return 1; // current chunk is from MMAP'd memory
    }

    return 0; // current chunk comes from MAIN ARENA & MAIN HEAP
    
}


