#include <fstream>
#include <iostream>
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <sstream>
#include <string>

using namespace std;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

struct shaderProgram {
  string vertexShader;
  string fragmentShader;
};

static shaderProgram parseShader(const string& filePath) {
  ifstream stream(filePath);

  enum class ShaderType {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
  };

  string line;
  stringstream ss[2];
  ShaderType type = ShaderType::NONE;

  while (getline(stream, line))
    {
      if (line.find("#shader") != string::npos)
	{
	  if (line.find("vertex") != string::npos)
	    type = ShaderType::VERTEX;
	  else if (line.find("fragment") != string::npos)
	    type = ShaderType::FRAGMENT;
	}
      else
	ss[(int)type] << line << '\n';
      
    }
  return {ss[0].str(), ss[1].str()}; 
}

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

  shaderProgram source = parseShader("./shaders/ocean.glsl");

  cout << "Vertex: " << source.vertexShader << '\n' << "Fragment: " << source.fragmentShader << endl;

  while (!glfwWindowShouldClose(window))
    {
      processInput(window);

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  
  glfwTerminate();
  return 0;
}
