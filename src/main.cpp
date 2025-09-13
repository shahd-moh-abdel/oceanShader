#include <fstream>
#include <iostream>
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <sstream>
#include <string>

#include "../include/shaders.h"
#include "../include/imguiInit.h"

using namespace std;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

float waveSpeed = 1.0f;
float waveHeight = 0.5f;

float sunHeight = 0.8f;
float fresnelPower = 2.0f;
float reflectionStrength = 0.8f;
float skyColor1[3] = {0.5f, 0.7f, 1.0f};
float skyColor2[3] = {0.7f, 0.9f, 1.0f};
float waterColor[3] = {0.1f, 0.3f, 0.6f};

void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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

  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);  

  return window;
}

int main()
{
  GLFWwindow* window = initWindow();
  initImGui(window);

  createFullScreen();
  
  shaderProgram source = parseShader("./shaders/ocean.glsl");
  unsigned int shader = createShader(source.vertexShader, source.fragmentShader);
  glUseProgram(shader);

  int sunHeightLoc = glGetUniformLocation(shader, "u_sunHeight");
  if (sunHeightLoc != -1)
    glUniform1f(sunHeightLoc, sunHeight);
  
  int colorLoc = glGetUniformLocation(shader, "u_color");
  if (colorLoc != -1)
    glUniform3f(colorLoc, waterColor[0], waterColor[1], waterColor[2]);

  int speedLoc = glGetUniformLocation(shader, "u_waveSpeed");
  if (speedLoc != -1)
    glUniform1f(speedLoc, waveSpeed);

  int heightLoc = glGetUniformLocation(shader, "u_waveHeight");
  if (speedLoc != -1)
    glUniform1f(heightLoc, waveHeight);

  int resLoc = glGetUniformLocation(shader, "u_resolution");
  if (resLoc != -1)
    glUniform2f(resLoc, SCREEN_WIDTH, SCREEN_HEIGHT);
  
  int timeLoc = glGetUniformLocation(shader, "u_time");

  double startTime = glfwGetTime();
  
  while (!glfwWindowShouldClose(window))
    {
      processInput(window);

      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      float currentTime = glfwGetTime() - startTime;
      if (timeLoc != -1)
	glUniform1f(timeLoc, currentTime); 
      
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
     
      renderImGui(shader, waterColor, waveSpeed, waveHeight, sunHeight);
      
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

  cleanup();
  glfwTerminate();
  return 0;
}
