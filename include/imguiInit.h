#ifndef IMGUI_INIT_H
#define IMGUI_INIT_H

#include <GLFW/glfw3.h>

void initImGui(GLFWwindow* window); 
void renderImGui(unsigned int shader, float skyColor[], float skyColor1[], float skyColor2[], float& waveSpeed, float& waveHeight, float& sunHeight);
void cleanup();

#endif
