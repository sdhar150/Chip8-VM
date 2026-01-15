# ============================================================
# CHIP-8 Emulator - Cross Platform Makefile
# Author: Soham Dhar
# ============================================================

# Compiler and Flags
CXX       := g++
CXXFLAGS  := -std=c++17 -Wall -Wextra -O2 -Iinclude

# Platform detection
UNAME_S := $(shell uname -s)

ifeq ($(OS),Windows_NT)
    LIBS := -lmingw32 -lSDL2main -lSDL2 -lopengl32
    RM   := del /Q
    EXE  := .exe
else ifeq ($(UNAME_S),Darwin)
    LIBS := -F /Library/Frameworks -framework SDL2 -framework OpenGL
    RM   := rm -f
    EXE  :=
else
    LIBS := -lSDL2 -lGL
    RM   := rm -f
    EXE  :=
endif

# Directories
SRC_DIR   := src
BUILD_DIR := build
BIN_DIR   := bin
INCLUDE   := include

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Output
TARGET := $(BIN_DIR)/chip8$(EXE)

# -------------------------------
# Build Rules
# -------------------------------
.PHONY: all clean debug release run dirs

all: dirs $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking: $@"
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling: $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

dirs:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)

debug: CXXFLAGS := -std=c++17 -Wall -Wextra -g -I$(INCLUDE)
debug: clean all

release: CXXFLAGS := -std=c++17 -Wall -Wextra -O3 -I$(INCLUDE)
release: clean all

run: $(TARGET)
	./$(TARGET)

clean:
	@echo "Cleaning..."
	-$(RM) $(OBJS) $(TARGET)
