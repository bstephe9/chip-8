# Variables and flags
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = `sdl2-config --cflags --libs` -lSDL2_Mixer
EMFLAGS = -sUSE_SDL=2 -sUSE_SDL_MIXER=2

# Directories
SRC_DIR = src
BUILD_DIR = build
TESTS_DIR = tests
UNITY_DIR = $(TESTS_DIR)/unity

# Files
TARGET = main
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))
CLEAN_FILES = *.exe *.o *.html *.wasm
TEST_FILE = $(TESTS_DIR)/test_chip8.c
TEST_TARGET = test_chip8

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)  # Ensure the build directory exists
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS)

tests: $(TEST_TARGET)

# Generate emcc js/wasm files and move to public/
local: $(SRC_FILES)
	echo "var Module = { canvas: document.getElementById('canvas') };" > temp_pre.js
	emcc $(SRC_FILES) -s -o $(TARGET).js $(CFLAGS) $(EMFLAGS) --pre-js temp_pre.js
	rm temp_pre.js
	# Remove local paths in file
	sed '/^\/\/ include: C:.*/d; /^\/\/ end include: C:.*/d' $(TARGET).js > cleaned.js
	mv cleaned.js $(TARGET).js
	mv $(TARGET).js $(TARGET).wasm ../public

$(TEST_TARGET): $(TEST_FILE)
	$(CC) $(CFLAGS) -o $@ $(SRC_DIR)/chip8.c $(UNITY_DIR)/unity.c $^ $(LDFLAGS)
	./$(TEST_TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(TEST_TARGET)

.PHONY: all clean debug
