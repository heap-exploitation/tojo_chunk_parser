#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct chunk_metadata {

    char *chunk_addr;
    size_t chunk_size;
    size_t is_main_arena;
    size_t is_mmapped;
    size_t is_chunk_free;
    char *next_chunk;
    size_t next_chunk_size;
    size_t is_prev_chunk_free;
};



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
int chunk_free(char *ptr, struct chunk_metadata *metadata){

    char *current_chunk_addr = ptr - sizeof(size_t); // Go back 8 bytes
                                                     
    size_t chunk_size = *(size_t *)current_chunk_addr & ~7;
    char *next_chunk = current_chunk_addr + chunk_size;
    size_t next_chunk_size = *(size_t *)next_chunk;
    size_t next_chunk_amP = next_chunk_size & 1;

    metadata->is_chunk_free = next_chunk_amP;
    metadata->chunk_addr = current_chunk_addr;

    return next_chunk_amP;
    

}

/* Function that tells the size of the chunk*/
void get_chunk_size(char *ptr, struct chunk_metadata *metadata) {

    char *current_chunk_addr = ptr - sizeof(size_t);

    size_t chunk_size = *(size_t *) current_chunk_addr & ~7;

    metadata->chunk_size = chunk_size;
}

/* Function that tells whether the prev chunk is free
 */
void prev_chunk_free(char *ptr, struct chunk_metadata *metadata ){

    char *current_chunk_addr = ptr - sizeof(size_t);

    size_t chunk_size = *(size_t *) current_chunk_addr & ~7;

    // prev chunk address
    char *prev_chunk_addr = current_chunk_addr - chunk_size;
    // get the prev chunk size
    size_t prev_chunk_size = *(size_t *)prev_chunk_addr & ~7;
    // calculate the address for the chunk below the prev
    char *below_prev_chunk_addr = prev_chunk_addr + prev_chunk_size;
    // get the size
    size_t below_prev_chunk_size = *(size_t *)below_prev_chunk_addr;
    size_t below_prev_chunk_amP = below_prev_chunk_size & 1;



    //char *next_chunk = current_chunk_addr + chunk_size;
    //size_t next_chunk_size = *(size_t *) next_chunk & ~7;




    //char *prev_chunk = next_chunk + next_chunk_size;
    //size_t prev_chunk_size = *(size_t *) prev_chunk;
    //size_t prev_chunk_amP = prev_chunk_size & 1;


    metadata->is_prev_chunk_free = below_prev_chunk_amP;

}


/* Function that tells whether the chunk is mmapped */
void is_mmapped(char *ptr, struct chunk_metadata *metadata){

    char *current_chunk_addr = ptr - sizeof(size_t);

    size_t chunk_size = *(size_t *)current_chunk_addr & ~7;
    char *next_chunk = current_chunk_addr + chunk_size;
    size_t next_chunk_size = *(size_t *) next_chunk;
    size_t next_chunk_aMp = next_chunk_size & 2;

    metadata->is_mmapped = next_chunk_aMp;

}


/* Fuction that tells where the chunk comes from */
void non_main_arena(char *ptr, struct chunk_metadata *metadata){

    char *current_chunk_addr = ptr - sizeof(size_t);
    size_t chunk_size = *(size_t *)current_chunk_addr & ~7;
    char *next_chunk = current_chunk_addr + chunk_size;
    size_t next_chunk_size = *(size_t *) next_chunk;
    size_t next_chunk_Amp = next_chunk_size & 4;

    metadata->is_main_arena = next_chunk_Amp;
 
}


/* Function to write out meta-data */
void write_dump(FILE *file, struct chunk_metadata *metadata) {


    fprintf(file, "Current chunk address: %p\n", metadata->chunk_addr);
    fprintf(file, "Chunk size is: %zu\n", metadata->chunk_size);
    fprintf(file, "Chunk A flag is: %zu. IS_MAIN_ARENA? %s\n", metadata->is_main_arena, metadata->is_main_arena == 0 ? "Yes" : "No, Comes from MMAP"         );
    fprintf(file, "Chunk M flag is: %zu. IS_MMAPPED? %s\n", metadata->is_mmapped, metadata->is_mmapped == 1 ? "Yes" : "No");
    fprintf(file, "Prev Chunk P flag is: %zu. Is Chunk free? %s\n", metadata->is_chunk_free, metadata->is_chunk_free == 0 ? "Yes" : "No");
    fprintf(file, "Is Prev Chunk free: %s\n", metadata->is_prev_chunk_free == 0 ? "Yes" : "No");

}



int main (void){


    // Open a file to dump meta-data into
    FILE *file = fopen("chunk_dump.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    // create the metadata strcut
    struct chunk_metadata metadata;

    
    char *ptr;
    // call malloc
    ptr = malloc(0x10); // malloc returns a pointer to the payload section of the chunk
    strcpy ( ptr, "panda");

    // Print the chunk metadata in order 
    // First check if it is free or not
    int is_free = chunk_free(ptr, &metadata);
    if ( is_free == 1){
       get_chunk_size(ptr, &metadata);
       prev_chunk_free(ptr, &metadata);
       is_mmapped(ptr, &metadata);
       non_main_arena(ptr, &metadata);


    }
    else { // dump meta-data for freed chunks
    // Todo: dump meta-data for freed chunks
        ;
    }

    write_dump(file, &metadata);

    fclose(file);

}
