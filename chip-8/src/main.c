#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "chip8.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: chip8.exe <rom_name>\n");
        exit(EXIT_FAILURE);
    }

    chip8_t chip8;
    initialize(&chip8);
    if (!setup_graphics(&chip8.sdl)) exit(EXIT_FAILURE);

    char *rom_path = argv[1];
    if (!read_rom(&chip8.memory[PC_START], rom_path)) exit(EXIT_FAILURE);

    while (chip8.state == RUNNING) {
        handle_input(&chip8);
        emulate_cycle(&chip8);
    }

    cleanup(&chip8.sdl);

    return 0;
}
