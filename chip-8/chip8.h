/*
Properties and methods used by CHIP-8.
*/

#ifndef CHIP8_H
#define CHIP8_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define WINDOW_X 0
#define WINDOW_Y 50
#define WINDOW_SCALE 13

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

#define SOUND_PATH "chip-8/sounds/beep.wav"
#define FONT_PATH "chip-8/fonts/ARCADECLASSIC.TTF"

#define MEMORY_SIZE 4096

#define PC_START 0x200
#define PC_END 0xFFF
#define MAX_ROM_SIZE (PC_END - PC_START)  // 3,583 bytes

#define FONT_HEIGHT 5
#define FONT_START 0x50
#define FONT_END 0xA0
#define FONT_MEMORY_SIZE (FONT_END - FONT_START)  // 80 bytes

#define DEFAULT_PC_INCREMENT 2

extern const SDL_Color BLACK;
extern const SDL_Color WHITE;
extern const SDL_Color RED;

// SDL Object
typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    Mix_Chunk *sound;
    TTF_Font *font;
    SDL_Point paused_text_pos;
} sdl_t;

// CHIP-8 States
typedef enum { RUNNING, PAUSED, QUIT } state_t;

// CHIP-8 Object
typedef struct {
    uint16_t pc;      // Program counter
    uint16_t opcode;  // Current opcode
    uint16_t idx;     // Index register
    uint16_t sp;      // Stack pointer

    uint8_t V[16];                                 // V-registers (V0-VF)
    uint16_t stack[16];                            // Stack (16 levels)
    uint8_t memory[MEMORY_SIZE];                   // Memory (size = 4k)
    bool display[DISPLAY_WIDTH * DISPLAY_HEIGHT];  // Graphics
    bool keypad[16];                               // Keypad

    uint8_t delay_timer;  // Delay timer
    uint8_t sound_timer;  // Sound timer

    state_t state;  // Current running state
    sdl_t sdl;      // SDL object

    bool draw;  // Draw flag
} chip8_t;

void initialize(chip8_t *chip8);
long get_rom_size(FILE *fp);
bool read_rom(uint8_t *buffer, const char *rom_path);
bool setup_sdl(sdl_t *sdl);
void mainloop(void);
void handle_input(chip8_t *chip8);
void process_frame(void);
void emulate_cycle(chip8_t *chip8);
void update_display(chip8_t *chip8);
void draw_text(const char *text, SDL_Color color, SDL_Point pos, sdl_t *sdl);
void update_timers(chip8_t *chip8);
void cleanup(sdl_t *sdl);
void reload(const char *rom);

#endif /* CHIP8_H */