#include <fstream>
#include <iostream>
#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <sstream>
#include <string>


#include "../include/imgui/imgui.h"
#include "../include/imgui/imgui_impl_glfw.h"
#include "../include/imgui/imgui_impl_opengl3.h"

using namespace std;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

float waveSpeed = 1.0f;
float waveHeight = 0.5f;

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

  ImGui::StyleColorsLight();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

}

void renderImGui()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::Begin("Controls");
  ImGui::Text("this is a text");
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
  
  shaderProgram source = parseShader("./shaders/ocean.glsl");

  cout << "Vertex: " << source.vertexShader << '\n' << "Fragment: " << source.fragmentShader << endl;

  while (!glfwWindowShouldClose(window))
    {
      processInput(window);
      renderImGui();
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

  cleanup();
  glfwTerminate();
  return 0;
}
