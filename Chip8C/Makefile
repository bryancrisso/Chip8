# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g -Iinc

LINKER_FLAGS = -lSDL2 -lSDL2main -lm

# Source files
SRCS = src/main.c src/chip8.c src/peripherals.c

# Object files
OBJS = $(SRCS:src/%.c=out/%.o)

# Executable name
TARGET = emulator.out

# Default target
all: $(TARGET)

# Compile source files into object files
out/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LINKER_FLAGS) 

# Link object files into executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LINKER_FLAGS)

# Clean up object files and executable
clean:
	rm -f $(OBJS) $(TARGET)