#include <fstream>
#include <iostream>
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <sstream>
#include <string>

#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui_impl_glfw.h"
#include "../include/imgui/imgui_impl_opengl3.h"
#include "../include/shaders.h"

using namespace std;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

float waveSpeed = 1.0f;
float waveHeight = 0.5f;
float skyColor[3] = {0.7, 0.7, 0.7};
void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

GLFWwindow* initWindow()
{
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  glfwInit();
  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Ocean", NULL, NULL);

  //error creating the window
  if (window == GL_FALSE)
    {
      cout << "Error Creating window" << endl;
      return nullptr;
    }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      cout << "Error: glad" << endl;
      return nullptr;
    }
  return window;
}

// creating imgui context 
void initImGui(GLFWwindow* window)
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;

  ImGui::StyleColorsClassic();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

}

void renderImGui(unsigned int shader)
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

int main()
{
  GLFWwindow* window = initWindow();
  initImGui(window);

  createFullScreen();
  
  shaderProgram source = parseShader("./shaders/ocean.glsl");
  unsigned int shader = createShader(source.vertexShader, source.fragmentShader);
  glUseProgram(shader);

  int colorLoc = glGetUniformLocation(shader, "u_color");
  if (colorLoc != -1)
    glUniform3f(colorLoc, skyColor[0], skyColor[1], skyColor[2]);

  while (!glfwWindowShouldClose(window))
    {
      processInput(window);

      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

      int speedLoc = glGetUniformLocation(shader, "u_waveSpeed");
      if (speedLoc != -1)
	glUniform1f(speedLoc, waveSpeed);

      int heightLoc = glGetUniformLocation(shader, "u_waveHeight");
      if (speedLoc != -1)
	glUniform1f(heightLoc, waveHeight);
	
      renderImGui(shader);
      
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

  cleanup();
  glfwTerminate();
  return 0;
}
