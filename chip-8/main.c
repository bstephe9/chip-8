#include <SDL.h>
#include <SDL_mixer.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "chip8.h"

chip8_t chip8;

int main(int argc, char *argv[]) {
    (void)argc;
#ifndef __EMSCRIPTEN__
    if (argc < 2) {
        fprintf(stderr, "Usage: chip8.exe <rom_name>\n");
        exit(EXIT_FAILURE);
    }
#endif

    initialize(&chip8);
    if (!setup_sdl(&chip8.sdl))
        exit(EXIT_FAILURE);

    char *rom_path = argv[1];
    if (!read_rom(&chip8.memory[PC_START], rom_path))
        exit(EXIT_FAILURE);

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainloop, 0, true);
#else
    while (1) {
        mainloop();
    }
#endif
    return 0;
}

void mainloop(void) {
    uint64_t start_time = SDL_GetPerformanceCounter();
    handle_input(&chip8);

    switch (chip8.state) {
        case RUNNING: process_frame(); break;
        case PAUSED: Mix_HaltChannel(-1); break;
        case QUIT: cleanup(&chip8.sdl); break;
#ifdef __EMSCRIPTEN__
            emscripten_cancel_main_loop();
#else
            exit(0);
#endif
    }

    update_display(&chip8);

    uint64_t end_time = SDL_GetPerformanceCounter();
    double elapsed_time =
        (end_time - start_time) / (double)SDL_GetPerformanceFrequency();

    // Delay for the remainder of this current frame
    double delay_amount = 16.67f - elapsed_time;
    if (delay_amount > 0) {
        SDL_Delay(delay_amount);
    }
}

void process_frame() {
    // 11 instructions per frame = 660 instructions per second
    for (size_t i = 0; i < 11; i++) {
        emulate_cycle(&chip8);

        // If draw instruction, break to only draw once during this frame
        if ((chip8.opcode >> 12) == 0xD)
            break;
    }

    update_timers(&chip8);
}

void reload(const char *rom) {
    initialize(&chip8);
    if (!setup_sdl(&chip8.sdl))
        exit(EXIT_FAILURE);
    if (!read_rom(&chip8.memory[PC_START], rom))
        exit(EXIT_FAILURE);
}