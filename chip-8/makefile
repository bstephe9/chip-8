# Variables and flags
CC = gcc
CFLAGS = `sdl2-config --cflags` -Wall -Wextra -g
LDFLAGS = `sdl2-config --libs`

# Directories
SRC_DIR = src
BUILD_DIR = build
TESTS_DIR = tests
UNITY_DIR = $(TESTS_DIR)/unity

# Files
TARGET = main
SRC_FILES = main.c chip8.c
OBJS = $(SRC_FILES:%.c=$(BUILD_DIR)/%.o)
CLEAN_FILES = *.exe *.o *.html *.wasm
TEST_FILE = $(TESTS_DIR)/test_chip8.c
TEST_TARGET = test_chip8

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)  # Ensure the build directory exists
	$(CC) -c $< -o $@ $(CFLAGS)

debug: $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS) -DDEBUG

tests: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_FILE)
	$(CC) $(CFLAGS) -o $@ $(SRC_DIR)/chip8.c $(UNITY_DIR)/unity.c $^ $(LDFLAGS)
	./$(TEST_TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(TEST_TARGET)

.PHONY: all clean debug
