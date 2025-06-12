# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -g -std=c++17

# SFML flags (adjust if SFML is in a custom path)
SFML_LIBS = -lsfml-system -lsfml-graphics -lsfml-window -lsfml-audio 

INCLUDES = -I"./Sources" \
		   -I"./Sources/Animation" \
		   -I"./Sources/Configuration" \
		   -I"./Sources/Enemy" \
		   -I"./Sources/Map" \
		   -I"./Sources/Npc" \
		   -I"./Sources/Player" \
		   -I"./Sources/Projectile" \
		   -I"./Sources/Quest" \
		   -I"./Sources/SoundManager"

# Directories
SRC_DIR   = Sources
BUILD_DIR = Build
TARGET    = ./Build/Demo.exe

# Find all .cpp files in SRC_DIR
SRCS := $(wildcard $(SRC_DIR)/*.cpp) \
        $(wildcard $(SRC_DIR)/Animation/*.cpp) \
        $(wildcard $(SRC_DIR)/Configuration/*.cpp) \
        $(wildcard $(SRC_DIR)/Enemy/*.cpp) \
        $(wildcard $(SRC_DIR)/Map/*.cpp) \
        $(wildcard $(SRC_DIR)/Npc/*.cpp) \
        $(wildcard $(SRC_DIR)/Player/*.cpp) \
        $(wildcard $(SRC_DIR)/Projectile/*.cpp) \
        $(wildcard $(SRC_DIR)/Quest/*.cpp) \
        $(wildcard $(SRC_DIR)/SoundManager/*.cpp) 

# Generate corresponding .o filenames
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Default target
all: $(TARGET)

# Link object files into binary
$(TARGET): $(OBJS)
	@echo Linking...
	@$(CXX) $(CXXFLAGS) -o $@ $^ $(SFML_LIBS) $(INCLUDES)

# Compile each .cpp
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo Compiling: $<
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -MMD -c $< -o $@ $(INCLUDES)

# Run the program
run: $(TARGET)
	$(TARGET)

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Include dependency files
DEPS := $(OBJS:.o=.d)
-include $(DEPS)
