#include <stdio.h>

#include "chip8.h"

int main()
{
    const int mem_size = 4096;

    unsigned char memory[4096] = {0};

    load_rom("/home/bryanak/Documents/fun/Chip8/ROMGenerator/test2.ch8", memory, 0x200);

    for (int i = 0; i < mem_size; i++)
    {
        printf("%x|", memory[i]);
    }
}


/**
 * Load a ROM file into memory
 */
int load_rom(char *filename, unsigned char *memory_buf, int start_loc)
{
    // open file pointer
    FILE *fp = fopen(filename, "rb");
    
    // check for error
    if (fp == NULL)
    {
        fprintf(stderr, "Error: Couldn't open file %s\n", filename);
        fclose(fp);
        return 1;
    }
    
    // seek to the end of the file (work out filesize)
    fseek(fp, 0, SEEK_END);
    unsigned long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // read file into memory buffer starting at start_loc
    size_t bytes_read = fread(memory_buf + start_loc, 1, file_size-start_loc, fp);

    fclose(fp);

    // check if we had a read error
    if (bytes_read != file_size)
    {
        fprintf(stderr, "Error: Couldn't read file %s\n", filename);
        fprintf(stderr, "File size: %lu\nBytes read: %zu\n", file_size, bytes_read);
        return 1;
    }

    // return no error code if we had no error !
    return 0;
}
