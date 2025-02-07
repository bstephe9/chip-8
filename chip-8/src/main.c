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
    if (!setup_graphics(&chip8.sdl))
        exit(EXIT_FAILURE);

    char *rom_path = argv[1];
    if (!read_rom(&chip8.memory[PC_START], rom_path))
        exit(EXIT_FAILURE);

    while (chip8.state == RUNNING) {
        uint64_t start_time = SDL_GetPerformanceCounter();

        handle_input(&chip8);

        // 11 instructions per frame = 660 instructions per second
        for (size_t i = 0; i < 11; i++) {
            emulate_cycle(&chip8);

            // If draw instruction, break to only draw once during this frame
            if ((chip8.opcode >> 12) == 0xD)
                break;
        }

        if (chip8.draw) {
            update_display(&chip8);
            chip8.draw = false;
        }

        update_timers(&chip8);

        uint64_t end_time = SDL_GetPerformanceCounter();
        double elapsed_time =
            (end_time - start_time) / (double)SDL_GetPerformanceFrequency();

        // Delay for the remainder of this current frame
        double delay_amount = 16.67f - elapsed_time;
        if (delay_amount > 0) {
            SDL_Delay(delay_amount);
        }
    }

    cleanup(&chip8.sdl);

    return 0;
}
