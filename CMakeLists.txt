cmake_minimum_required(VERSION 3.10)

# === CONFIG ===
project(MyProject)

# Detect platform
if(WIN32)
    set(SYS_LIBS opengl32 gdi32 user32)
    set(PLATFORM WINDOWS)
else()
    set(SYS_LIBS GL dl m pthread)
    set(PLATFORM LINUX)
endif()

# Compiler
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# Project flags
set(PROJECT_FLAGS -DCIMGUI_USE_OPENGL3 -DCIMGUI_USE_GLFW)
set(PROJECT_CXXFLAGS "-DIMGUI_IMPL_API=extern \"C\"")

# Include directories
include_directories(
    src
    vendor
    include
    vendor/cimgui
    vendor/cimgui/imgui
    vendor/cimgui/imgui/backends
)

# Source files
file(GLOB C_SRC src/*.c)
file(GLOB CXX_SRC vendor/cimgui/cimgui.cpp
                  vendor/cimgui/imgui/imgui*.cpp
                  vendor/cimgui/imgui/backends/imgui_impl_glfw.cpp
                  vendor/cimgui/imgui/backends/imgui_impl_opengl3.cpp)

# Set up debug and release object files
set(C_OBJ_DEBUG ${C_SRC} ${CXX_SRC})
set(CXX_OBJ_DEBUG ${CXX_SRC})

# === Build directories ===
set(BUILD_DIR ${CMAKE_BINARY_DIR}/build)
set(TARGET_DEBUG ${BUILD_DIR}/debug)
set(TARGET_RELEASE ${BUILD_DIR}/release)

# === Debug and Release flags ===
set(DEBUG_FLAGS -DGDEBUG)

# === Rules ===
add_executable(${TARGET_DEBUG} ${C_OBJ_DEBUG} ${CXX_OBJ_DEBUG})
target_compile_options(${TARGET_DEBUG} PRIVATE ${CFLAGS} ${PROJECT_FLAGS} ${DEBUG_FLAGS})
target_link_libraries(${TARGET_DEBUG} ${SYS_LIBS} glfw3)

add_executable(${TARGET_RELEASE} ${C_OBJ_DEBUG} ${CXX_OBJ_DEBUG})
target_compile_options(${TARGET_RELEASE} PRIVATE ${CFLAGS} ${PROJECT_FLAGS})
target_link_libraries(${TARGET_RELEASE} ${SYS_LIBS} glfw3)

# === Clean ===
add_custom_target(clean
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_clean.cmake
)

