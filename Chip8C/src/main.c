#include <stdio.h>
#include <stdlib.h>

#include <chip8.h>
#include <peripherals.h>

int load_rom(char *filename, unsigned char *memory_buf, int start_loc);

int main()
{
    struct chip8 *c8 = malloc(sizeof(struct chip8));

    int should_quit = 0;

    // load_rom("/home/bryanak/Documents/fun/Chip8/ROMGenerator/test_opcode.ch8", c8->memory, 0x200);
    load_rom("/home/bryanak/Documents/fun/chip8-roms/games/Airplane.ch8", c8->memory, 0x200);

    init(c8);

    // start an output window

    init_display(640, 320, 64, 32);

    // FDE Cycle

    while (should_quit != -1)
    {
        should_quit = cycle(c8);
        if (sdl_ehandler(c8->keys) == -1) should_quit = -1;

        if (should_quit == DRAW_FLAG)
        {
            draw(c8->display_buf);
        }

        nanosleep(&ts, &ts);
    }

    free(c8);
    stop_display();
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
