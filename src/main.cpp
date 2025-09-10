#include <iostream>
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>

using namespace std;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

GLFWwindow* initWindow()
{
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

int main()
{
  GLFWwindow* window = initWindow();

  while (!glfwWindowShouldClose(window))
    {
      processInput(window);

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  
  glfwTerminate();
  return 0;
}
