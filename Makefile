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
PROJECT_FLAGS := -DCIMGUI_USE_OPENGL3 -DCIMGUI_USE_GLFW
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
		 $(wildcard $(INCLUDE_DIR)/*.c) \
		$(wildcard $(SRC_DIR)/test/*.c)

CXX_SRC := $(wildcard $(VENDOR_DIR)/cimgui/cimgui*.cpp) \
           $(wildcard $(VENDOR_DIR)/cimgui/imgui/imgui*.cpp) \
           $(wildcard $(VENDOR_DIR)/cimgui/imgui/backends/imgui_impl_glfw.cpp) \
           $(wildcard $(VENDOR_DIR)/cimgui/imgui/backends/imgui_impl_opengl3.cpp)

# Object files for debug and release
C_OBJ_DEBUG := $(C_SRC:%.c=$(BUILD_DIR)/debug/%.o)
CXX_OBJ_DEBUG := $(CXX_SRC:%.cpp=$(BUILD_DIR)/debug/%.o)
C_OBJ_RELEASE := $(C_SRC:%.c=$(BUILD_DIR)/release/%.o)
CXX_OBJ_RELEASE := $(CXX_SRC:%.cpp=$(BUILD_DIR)/release/%.o)

# Final binary names for debug and release
TARGET_DEBUG := $(BUILD_DIR)/debug
TARGET_RELEASE := $(BUILD_DIR)/release

# Debug flags
DEBUG_FLAGS := -DGC_DEBUG

# === RULES ===

all: $(TARGET_DEBUG) $(TARGET_RELEASE)

# Debug build
$(TARGET_DEBUG): $(C_OBJ_DEBUG) $(CXX_OBJ_DEBUG)
	@mkdir -p $(BUILD_DIR)/debug
	$(CXX) $(CFLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS) $(DEBUG_FLAGS) 
	@echo "Debug build complete: $(TARGET_DEBUG)"

# Release build
$(TARGET_RELEASE): $(C_OBJ_RELEASE) $(CXX_OBJ_RELEASE)
	@mkdir -p $(BUILD_DIR)/release
	$(CXX) $(CFLAGS) $(INCLUDES) $^ -o $@ $(LDFLAGS)
	@echo "Release build complete: $(TARGET_RELEASE)"

# Compile C files with gcc (debug version)
$(BUILD_DIR)/debug/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ $(PROJECT_FLAGS) $(DEBUG_FLAGS)
	@touch $@

# Compile C++ files with g++ (debug version)
$(BUILD_DIR)/debug/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@ $(PROJECT_FLAGS) $(DEBUG_FLAGS) $(PROJECT_CXXFLAGS) 
	@touch $@

# Compile C files with gcc (release version)
$(BUILD_DIR)/release/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ $(PROJECT_FLAGS)
	@touch $@

# Compile C++ files with g++ (release version)
$(BUILD_DIR)/release/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@ $(PROJECT_FLAGS) $(PROJECT_CXXFLAGS)
	@touch $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
