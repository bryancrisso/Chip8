# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g -Iinc

# Source files
SRCS = src/main.c

# Object files
OBJS = $(SRCS:src/%.c=out/%.o)

# Executable name
TARGET = emulator.out

# Default target
all: $(TARGET)

# Compile source files into object files
out/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJS) $(TARGET)