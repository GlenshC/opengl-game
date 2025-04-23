# === CONFIG ===

# Detect platform
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    SYS_LIBS := -lGL -ldl -lm -pthread 
    PLATFORM := LINUX
else ifeq ($(OS),Windows_NT)
    SYS_LIBS := -lopengl32 -lgdi32 -luser32 #-lshell32 -lkernel32 
    PLATFORM := WINDOWS
endif

# Compiler
CC := gcc
CXX := g++
CFLAGS := -Wall -Wextra
CXXFLAGS := -Wall -Wextra
PROJECT_FLAGS := -DCIMGUI_USE_OPENGL3 -DCIMGUI_USE_GLFW -DGDEBUG
PROJECT_CXXFLAGS := "-DIMGUI_IMPL_API=extern \"C\" "
# -DIMGUI_IMPL_API='extern "C"'
# Include directories (just the vendor root)
INCLUDES := -Isrc -Ivendor -Iinclude -Ivendor/cimgui -Ivendor/cimgui/imgui -Ivendor/cimgui/imgui/backends
LIBS := -Lvendor/GLFW -lglfw3

# Linker flags
LDFLAGS := $(LIBS) $(SYS_LIBS)

# Project structure
SRC_DIR := src
INCLUDE_DIR := include
VENDOR_DIR := vendor
BUILD_DIR := build

# Source files (your code + glad/glfw if using source)
C_SRC := $(wildcard $(SRC_DIR)/*.c) \
         $(wildcard $(VENDOR_DIR)/glad/*.c) \
         $(wildcard $(INCLUDE_DIR)/*.c)

         # $(wildcard $(VENDOR_DIR)/cimgui/*.c)
         # $(wildcard $(VENDOR_DIR)/cimgui/imgui/*.c) \
         # $(wildcard $(VENDOR_DIR)/cimgui/imgui/backends/*.c)

CXX_SRC := $(wildcard $(VENDOR_DIR)/cimgui/cimgui.cpp) \
           $(wildcard $(VENDOR_DIR)/cimgui/imgui/imgui*.cpp) \
           $(wildcard $(VENDOR_DIR)/cimgui/imgui/backends/imgui_impl_glfw.cpp) \
           $(wildcard $(VENDOR_DIR)/cimgui/imgui/backends/imgui_impl_opengl3.cpp)

# Object files
C_OBJ := $(C_SRC:%.c=$(BUILD_DIR)/%.o)
CXX_OBJ := $(CXX_SRC:%.cpp=$(BUILD_DIR)/%.o)

# Final binary name
TARGET := $(BUILD_DIR)/app

# === RULES ===

all: $(TARGET)

$(TARGET): $(C_OBJ) $(CXX_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CFLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS) $(PROJECT_FLAGS)
	@echo "Build complete: $(TARGET)"

# Compile C files with gcc
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ $(PROJECT_FLAGS)

# Compile C++ files with g++
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@ $(PROJECT_FLAGS) $(PROJECT_CXXFLAGS)  

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
