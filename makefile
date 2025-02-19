# Directories
SRC_DIR = src/chip-8
BUILD_DIR = build
TESTS_DIR = tests
UNITY_DIR = $(TESTS_DIR)/unity
PUBLIC_DIR = public

# Files
TARGET = chip8
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))
TEST_FILE = $(TESTS_DIR)/test_chip8.c
TEST_TARGET = test_chip8
CLEAN_FILES = $(BUILD_DIR) $(TARGET).exe $(TEST_TARGET).exe

# Variables and flags
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = `sdl2-config --cflags --libs` -lSDL2_Mixer
EMFLAGS = -sUSE_SDL=2 -sUSE_SDL_MIXER=2 --embed-file roms --embed-file sounds -sEXPORTED_RUNTIME_METHODS=callMain -sINVOKE_RUN=0

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS)

tests: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_FILE)
	$(CC) $(CFLAGS) -o $@ $(SRC_DIR)/chip8.c $(UNITY_DIR)/unity.c $^ $(LDFLAGS) -DUNIT_TEST
	./$(TEST_TARGET)
	rm $(TEST_TARGET)

# Generate emcc output
web: $(SRC_FILES)
	@mkdir -p $(PUBLIC_DIR)
	emcc $(SRC_FILES) -o $(PUBLIC_DIR)/$(TARGET).js $(CFLAGS) $(EMFLAGS)

clean:
	rm -rf $(CLEAN_FILES)

.PHONY: all clean debug
