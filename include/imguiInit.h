#ifndef IMGUI_INIT_H
#define IMGUI_INIT_H

#include <GLFW/glfw3.h>

void initImGui(GLFWwindow* window); 
void renderImGui(unsigned int shader, float skyColor[], float waveSpeed, float waveHeight);
void cleanup();

#endif
