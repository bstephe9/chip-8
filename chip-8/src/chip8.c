#include "chip8.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

unsigned char chip8_fontset[FONT_MEMORY_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
    0x20, 0x60, 0x20, 0x20, 0x70,  // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
    0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
    0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
    0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
    0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
    0xF0, 0x80, 0xF0, 0x80, 0x80   // F
};

void initialize(chip8_t *chip8) {
    // Registers
    chip8->pc = 0x200;
    chip8->opcode = 0x0;
    chip8->idx = 0x0;
    chip8->sp = 0x0;

    // V-Registers, stack, memory
    memset(chip8->V, 0, sizeof(chip8->V));
    memset(chip8->stack, 0, sizeof(chip8->stack));
    memset(chip8->memory, 0, sizeof(chip8->memory));

    // Graphics
    memset(chip8->gfx, 0, sizeof(chip8->gfx));
    memset(&chip8->sdl, 0, sizeof(chip8->sdl));

    // Load fontset into memory
    for (int i = 0; i < FONT_MEMORY_SIZE; i++) {
        chip8->memory[FONT_START + i] = chip8_fontset[i];
    }

    // Set initial state
    chip8->state = RUNNING;
}

bool read_rom(uint8_t *buffer, const char *rom_path) {
    FILE *rom = fopen(rom_path, "rb");
    if (!rom) {
        fprintf(stderr, "Unable to open rom\n");
        return false;
    }

    long rom_size = get_rom_size(rom);
    if (rom_size > MAX_ROM_SIZE) {
        fprintf(stderr, "Error: Rom size too large\n");
        return false;
    }

    // Read ROM into buffer
    if (fread(buffer, 1, rom_size, rom) == 0) {
        fprintf(stderr, "Error: Could not load ROM into memory\n");
        return false;
    }

    return true;
}

long get_rom_size(FILE *fp) {
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);
    return size;
}

bool setup_graphics(sdl_t *sdl) {
    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        printf("Could not initialize SDL: %s\n", SDL_GetError());
        return false;
    }

    sdl->window = SDL_CreateWindow("CHIP-8 Emulator", 200, 400, 320, 240, 0);
    if (sdl->window == NULL) {
        printf("Could not initialize SDL_Window: %s\n", SDL_GetError());
        return false;
    }

    sdl->renderer =
        SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED);
    if (sdl->renderer == NULL) {
        printf("Could not initialize SDL_Renderer: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void emulate_cycle(chip8_t *chip8) {
    // Fetch opcode
    chip8->opcode =
        chip8->memory[chip8->pc] << 8 | chip8->memory[chip8->pc + 1];

    // Increment program counter to next instruction
    chip8->pc += 2;

    uint16_t X = chip8->opcode & 0x0F00;
    uint16_t Y = chip8->opcode & 0x00F0;

    uint32_t N = chip8->opcode & 0x0F00;
    uint32_t NN = chip8->opcode & 0x0FF0;
    uint32_t NNN = chip8->opcode & 0x0FFF;

    // Decode and execute opcode
    switch (chip8->opcode & 0xF000) {
        case 0x0000:
            switch (chip8->opcode & 0x000F) {
                case 0x0000:  // 00E0; Clears the screen.
                    memset(chip8->gfx, 0, sizeof(chip8->gfx));
                    break;
                case 0x000E:  // 00EE: Returns from a subroutine.
                    chip8->sp--;
                    chip8->pc = chip8->stack[chip8->sp];
                    break;
                default:
            }
            break;
        case 0x1000:  // 1NNN; Jumps to address NNN.
            chip8->pc = NNN;
            break;
        case 0x2000:  // 2NNN; Calls subroutine at NNN.
            chip8->stack[chip8->sp] = chip8->pc;
            chip8->sp++;
            chip8->pc = NNN;
            break;
        case 0x3000:  // 3XNN; Skips the next instruction if VX equals NN.
            if (chip8->V[X] == NN)
                chip8->pc += 2;
            break;
        case 0x4000:  // 4XNN; Skips the next instruction if VX does not equal
                      // NN.
            if (chip8->V[X] != NN)
                chip8->pc += 2;
            break;
        case 0x5000:  // 5XY0; Skips the next instruction if VX equals VY.
            if (chip8->V[X] == chip8->V[Y])
                chip8->pc += 2;
        case 0xD000:  // DXYN; Draws a sprite at coordinate (VX, VY) that has a
                      // width of 8 pixels and a height of N pixels.
            // draw(Vx, Vy, N);
            break;
        case 0x8000:
            switch (chip8->opcode & 0x000F) {
                case 0x0001:  // 8XY1; Sets VX to VX or VY.
                    chip8->V[X] |= chip8->V[Y];
                    break;
                case 0x0002:  // 8XY2; Sets VX to VX and VY.
                    chip8->V[X] &= chip8->V[Y];
                    break;
                case 0x0003:  // 8XY3; Sets VX to VX xor VY.
                    chip8->V[X] ^= chip8->V[Y];
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

void handle_input(chip8_t *chip8) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                chip8->state = QUIT;
                return;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        chip8->state = QUIT;
                        return;
                }
                break;
            default:
                break;
        }
    }
}

void cleanup(sdl_t *sdl) {
    SDL_DestroyRenderer(sdl->renderer);
    SDL_DestroyWindow(sdl->window);
    sdl->window = NULL;
    sdl->renderer = NULL;
    SDL_Quit();
}
