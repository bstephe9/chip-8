/*
Properties and methods used by CHIP-8.
*/

#ifndef CHIP8_H
#define CHIP8_H

#include <SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define MEMORY_SIZE 4096

#define PC_START 0x200
#define PC_END 0xFFF
#define MAX_ROM_SIZE (PC_END - PC_START) // 3,583 bytes

#define FONT_START 0x50
#define FONT_END 0xA0
#define FONT_MEMORY_SIZE (FONT_END - FONT_START)  // 80 bytes

#define DEFAULT_PC_INCREMENT 2

// SDL Object
typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
} sdl_t;

// CHIP-8 States
typedef enum { RUNNING, PAUSED, QUIT } state_t;

// CHIP-8 Object
typedef struct {
    unsigned short pc;      // Program counter
    unsigned short opcode;  // Current opcode
    unsigned short idx;     // Index register
    unsigned short sp;      // Stack pointer

    unsigned char V[16];                // V-registers (V0-VF)
    unsigned short stack[16];           // Stack (16 levels)
    unsigned char memory[MEMORY_SIZE];  // Memory (size = 4k)
    unsigned char gfx[64 * 32];         // Graphics
    unsigned char keypad[16];           // Keypad

    unsigned char delay_timer;  // Delay timer
    unsigned char sound_timer;  // Sound timer

    state_t state;  // Current running state
    sdl_t sdl;      // SDL object
} chip8_t;

void initialize(chip8_t *chip8);
long get_rom_size(FILE *fp);
bool read_rom(unsigned char *buffer, const char *rom_path);
void setup_graphics(sdl_t *sdl);
void emulate_cycle(chip8_t *chip8);
void setup_graphics(sdl_t *sdl);
void handle_input(chip8_t *chip8);
void cleanup(sdl_t *sdl);

#endif /* CHIP8_H */