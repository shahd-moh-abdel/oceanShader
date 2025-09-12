#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>

#include "../include/imguiInit.h"
#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui_impl_glfw.h"
#include "../include/imgui/imgui_impl_opengl3.h"


void initImGui(GLFWwindow* window)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;

  ImGui::StyleColorsClassic();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");
}

void renderImGui(unsigned int shader, float skyColor[], float waveSpeed, float waveHeight)
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::Begin("Controls");

  if (ImGui::ColorEdit3("Sky color", skyColor))
    {
      int colorLoc = glGetUniformLocation(shader, "u_color");
      if (colorLoc != -1)
	glUniform3f(colorLoc, skyColor[0], skyColor[1], skyColor[2]);
    }
  
  ImGui::Text("Wave Speed: %.2f", waveSpeed);
  ImGui::SliderFloat("Speed", &waveSpeed, 0.0f, 5.0f);
  
  ImGui::Text("Wave Height: %.2f", waveHeight);
  ImGui::SliderFloat("Height", &waveHeight, 0.0f, 2.0f);
  ImGui::End();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void cleanup()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}


