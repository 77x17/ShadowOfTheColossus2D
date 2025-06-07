# Makefile for SFML 2D Pattern Game

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall

# SFML flags (adjust if SFML is in a custom path)
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Output binary
TARGET = demo

# Source files
SRCS = *.cpp

# Build target
all: $(TARGET)

# Run the program after building
run: $(TARGET)
	./$(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(SFML_LIBS)

# Clean up
clean:
	rm -f $(TARGET)
