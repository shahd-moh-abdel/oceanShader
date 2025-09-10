#!/bin/bash

g++ src/main.cpp \
    src/shaders.cpp \
    src/imgui/imgui.cpp \
    src/imgui/imgui_demo.cpp \
    src/imgui/imgui_draw.cpp \
    src/imgui/imgui_tables.cpp \
    src/imgui/imgui_widgets.cpp \
    src/imgui/imgui_impl_glfw.cpp \
    src/imgui/imgui_impl_opengl3.cpp \
    glad.c \
    -o app \
    -Iinclude \
    -Iinclude/imgui \
    -lglfw \
    -ldl \
    -lX11 \
    -lpthread \
    -lXrandr \
    -lXi
