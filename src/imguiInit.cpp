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

void renderImGui(unsigned int shader, float waterColor[], float sunColor[], float skyColor1[], float skyColor2[], float& waveSpeed, float& waveHeight, float& sunHeight)
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::Begin("Controls");

  if (ImGui::ColorEdit3("water Color", waterColor))
    {
      int waterColorLoc = glGetUniformLocation(shader, "u_waterColor");
      if (waterColorLoc != -1)
	glUniform3f(waterColorLoc, waterColor[0], waterColor[1], waterColor[2]);
    }

  if (ImGui::ColorEdit3("Sun Color", sunColor))
    {
      int sunColorLoc = glGetUniformLocation(shader, "u_sunColor");
      if (sunColorLoc != -1)
	glUniform3f(sunColorLoc, sunColor[0], sunColor[1], sunColor[2]);
    }
  
  if (ImGui::ColorEdit3("Sky Color 1", skyColor1))
    {
      int skyColor1Loc = glGetUniformLocation(shader, "u_skyColor1");
      if (skyColor1Loc != -1)
	glUniform3f(skyColor1Loc, skyColor1[0], skyColor1[1], skyColor1[2]);
    }
  
  if (ImGui::ColorEdit3("Sky Color 2", skyColor2))
    {
      int skyColor2Loc = glGetUniformLocation(shader, "u_skyColor2");
      if (skyColor2Loc != -1)
	glUniform3f(skyColor2Loc, skyColor2[0], skyColor2[1], skyColor2[2]);
    }

  ImGui::Text("Sun Height: %.2f", sunHeight);
  if (ImGui::SliderFloat("Sun height", &sunHeight, 0.0f, 1.5f))
    {
      int sunHeightLoc = glGetUniformLocation(shader, "u_sunHeight");
      if (sunHeightLoc != -1)
	glUniform1f(sunHeightLoc, sunHeight);

    }
  
  ImGui::Text("Wave Speed: %.2f", waveSpeed);
  if (ImGui::SliderFloat("Speed", &waveSpeed, 0.0f, 5.0f))
    {
      int speedLoc = glGetUniformLocation(shader, "u_waveSpeed");
      if (speedLoc != -1)
	glUniform1f(speedLoc, waveSpeed);

    }
  
  ImGui::Text("Wave Height: %.2f", waveHeight);
  if (ImGui::SliderFloat("Height", &waveHeight, 0.0f, 2.0f))
    {
      int heightLoc = glGetUniformLocation(shader, "u_waveHeight");
      if (heightLoc != -1)
	glUniform1f(heightLoc, waveHeight);
    }
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


