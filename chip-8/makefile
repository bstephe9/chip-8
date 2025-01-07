# Variables and flags
CC = gcc
SRC_FILES = main.c chip8.c
OBJS = $(SRC_FILES:%.c=$(BUILD_DIR)/%.o)
CLEAN_FILES = *.exe *.o *.html *.wasm
CFLAGS = `sdl2-config --cflags` -Wall -Wextra -g
LDFLAGS = `sdl2-config --libs`

# Directories
SRC_DIR = src
BUILD_DIR = build
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)  # Ensure the build directory exists
	$(CC) -c $< -o $@ $(CFLAGS)

debug: $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS) -DDEBUG

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean debug
