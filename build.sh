#!/bin/bash

CXX=g++
CXXFLAGS="-std=c++17 -Wall -Wextra -O2"
LDFLAGS="-ldl -lGL"

SRC_DIR="src"
BUILD_DIR="build"
VENDOR_DIR="vendor"
EXECUTABLE="game"

INCLUDE_FLAGS="-I$SRC_DIR -I$VENDOR_DIR -I$VENDOR_DIR/imgui"

SOURCES=$(find $SRC_DIR -type f -name "*.cpp")
GLAD_SRC="$VENDOR_DIR/glad/src/glad.c"
GLFW_LIB="$VENDOR_DIR/GLFW/lib/libglfw3.a"

IMGUI_SOURCES="
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/backends/imgui_impl_glfw.cpp
    vendor/imgui/backends/imgui_impl_opengl3.cpp
"

mkdir -p $BUILD_DIR

echo "Compiling..."
$CXX $CXXFLAGS $SOURCES $IMGUI_SOURCES $GLAD_SRC $GLFW_LIB $INCLUDE_FLAGS -o $BUILD_DIR/$EXECUTABLE $LDFLAGS

if [ $? -eq 0 ]; then
    echo "Build successful! Run with ./$BUILD_DIR/$EXECUTABLE"
else
    echo "Build failed."
fi
