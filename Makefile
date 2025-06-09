# Makefile for SFML 2D Pattern Game

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -I./src

# SFML flags (adjust if SFML is in a custom path)
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Directories
SRC_DIR = src
BUILD_DIR = build

# Output binary
TARGET = demo

# Find all .cpp files in SRC_DIR
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# Generate corresponding .o filenames in BUILD_DIR
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Default target
all: $(TARGET)

# Link object files into binary
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SFML_LIBS)

# Compile .cpp to .o and generate .d dependency file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Include dependency files
DEPS := $(OBJS:.o=.d)
-include $(DEPS)
