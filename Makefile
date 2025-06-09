# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -g

# SFML flags (adjust if SFML is in a custom path)
SFML_LIBS = -lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio 

# Directories
SRC_DIR = Sources
BUILD_DIR = Build

# Output binary
TARGET = Demo

# Find all .cpp files in SRC_DIR
SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# Generate corresponding .o filenames in BUILD_DIR
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Default target
all: $(TARGET)

# Link object files into binary
$(TARGET): $(OBJS)
	$(CXX) -g $(CXXFLAGS) -o $@ $^ $(SFML_LIBS)

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
