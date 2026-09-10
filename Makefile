# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Source files
SRC = $(SRC_DIR)/httpserver.c

# Output executable
TARGET = $(BUILD_DIR)/httpserver


# Default target
all: $(TARGET)


# Build the server
$(TARGET): $(SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $(SRC) -o $(TARGET)


# Run the server
run: $(TARGET)
	./$(TARGET)


# Clean build files
clean:
	rm -rf $(BUILD_DIR)


# Rebuild everything
rebuild: clean all


.PHONY: all run clean rebuild
