#include <stdio.h>

#include "../src/chip8.h"
#include "unity/unity.h"

chip8_t chip8;
char *rom_path = "tests/chip8_roms/IBM_Logo.ch8";
FILE *test_rom;

void setUp(void) {
    initialize(&chip8);
    test_rom = fopen("tests/chip8_roms/IBM_Logo.ch8", "rb");
}

void tearDown(void) {}

void test_should_zero_initialize(void) {
    initialize(&chip8);
    
    // Test registers
    TEST_ASSERT_EQUAL_HEX(0x200, chip8.pc);
    TEST_ASSERT_EQUAL_HEX(0x0, chip8.opcode);
    TEST_ASSERT_EQUAL_HEX(0x0, chip8.idx);
    TEST_ASSERT_EQUAL_HEX(0x0, chip8.sp);

    // Test V registers
    for (int i = 0; i < 16; i++) {
        TEST_ASSERT_EQUAL_UINT8(0, chip8.V[i]);
    }

    // Test stack
    for (int i = 0; i < 16; i++) {
        TEST_ASSERT_EQUAL_UINT16(0, chip8.stack[i]);
    }

    // Test memory
    for (int i = 0; i < MEMORY_SIZE; i++) {
        // Skip the font portion of chip8 memory because that is not supposed
        // to be zero-initialized
        if (FONT_START <= i && i <= FONT_END) continue;

        TEST_ASSERT_EQUAL_UINT8(0, chip8.memory[i]);
    }

    // Test graphics
    for (int i = 0; i < 64 * 32; i++) {
        TEST_ASSERT_EQUAL_UINT8(0, chip8.gfx[i]);
    }

    // Test SDL object
    TEST_ASSERT_EQUAL_PTR(NULL, chip8.sdl.window);
    TEST_ASSERT_EQUAL_PTR(NULL, chip8.sdl.renderer);
}

void test_should_get_correct_rom_file_size(void) {
    long rom_size = get_rom_size(test_rom);
    TEST_ASSERT_EQUAL(132, rom_size);
}

// Change 'main' to 'SDL_main' to avoid conflict with SDL2's entry point
int SDL_main(int argc, char *argv[]) {
    UNITY_BEGIN();
    RUN_TEST(test_should_zero_initialize);
    RUN_TEST(test_should_get_correct_rom_file_size);
    return UNITY_END();
}