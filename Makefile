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
CFLAGS := -Wall -Wextra -O3
PROJECT_FLAGS := 
DEBUG_FLAGS := -DGC_DEBUG

# Include directories (just the vendor root)
INCLUDES := -Isrc -Ivendor -Iinclude
LIBS := -Lvendor/GLFW -lglfw3

LDFLAGS := $(LIBS) $(SYS_LIBS)
# Linker flags

# Project structure
SRC_DIR := src
INCLUDE_DIR := include
VENDOR_DIR := vendor
BUILD_DIR := build

# Source files (your code + glad/glfw if using source)
C_SRC := $(wildcard $(SRC_DIR)/*.c) \
         $(wildcard $(VENDOR_DIR)/glad/*.c) \
		 $(wildcard $(INCLUDE_DIR)/*.c)

# Object files for debug and release
C_OBJ_DEBUG := $(C_SRC:%.c=$(BUILD_DIR)/debug/%.o)
C_OBJ_RELEASE := $(C_SRC:%.c=$(BUILD_DIR)/release/%.o)

# Final binary names for debug and release
TARGET_DEBUG := $(BUILD_DIR)/debug
TARGET_RELEASE := $(BUILD_DIR)/release

# Debug flags

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

# Compile C files with gcc (release version)
$(BUILD_DIR)/release/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@ $(PROJECT_FLAGS)
	@touch $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
