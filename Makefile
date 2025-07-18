# USB Device Enumerator Makefile
# Windows C Application using Windows API

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -municode -DUNICODE -D_UNICODE
LDFLAGS = -lsetupapi -ladvapi32 -luuid

# Target executable
TARGET = WinViewUSB.exe

# Source files
SOURCES = main.c
HEADERS = usb_enum.h

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build completed successfully!"

# Compile source files to object files
%.o: %.c $(HEADERS)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	del /Q *.o $(TARGET) 2>nul || echo "Nothing to clean."

# Clean and rebuild
rebuild: clean all

# Create a release build
release: CFLAGS += -DNDEBUG
release: clean $(TARGET)

# Create a debug build
debug: CFLAGS += -g -DDEBUG
debug: clean $(TARGET)

# Install dependencies (for reference - requires Windows SDK)
install-deps:
	@echo "Required dependencies:"
	@echo "- Windows SDK (for setupapi.h, devguid.h)"
	@echo "- MinGW-w64 or Visual Studio Build Tools"
	@echo "- Make utility (mingw32-make)"

# Help target
help:
	@echo "Available targets:"
	@echo "  all       - Build the application (default)"
	@echo "  clean     - Remove build artifacts"
	@echo "  rebuild   - Clean and build"
	@echo "  release   - Build optimized release version"
	@echo "  debug     - Build debug version with symbols"
	@echo "  help      - Show this help message"

# Run the application (for testing)
run: $(TARGET)
	@echo "Running $(TARGET)..."
	@./$(TARGET)

.PHONY: all clean rebuild release debug install-deps help run
