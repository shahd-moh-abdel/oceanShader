#include "../include/shaders.h"
#include "../include/glad/glad.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

GLuint g_VAO = 0;
GLuint g_VBO = 0;

shaderProgram parseShader(const string& filePath)
{
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

// Compile Shaders from a file
unsigned int compileShader(unsigned int type, const string& source) {
    GLuint id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    
    // Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        cout << "ERROR: error in " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") 
             << " Shader: " << message << endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

// Creating Shaders
unsigned int createShader(const string& vertexShader, const string& fragmentShader) {
    GLuint program = glCreateProgram();
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    return program;
}

void createFullScreen()
{
  GLfloat vertices[] = {
    -1.0f, -1.0f,
    +1.0f, -1.0f,
    -1.0f, +1.0f,
    +1.0f, +1.0f
  };

  GLuint VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
}

void cleanupFullScreen()
{
    if (g_VBO != 0) {
        glDeleteBuffers(1, &g_VBO);
        g_VBO = 0;
    }
    if (g_VAO != 0) {
        glDeleteVertexArrays(1, &g_VAO);
        g_VAO = 0;
    }
}
