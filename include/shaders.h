#ifndef SHADERS_H
#define SHADERS_H

#include <string>

struct shaderProgram {
  std::string vertexShader;
  std::string fragmentShader;
};

shaderProgram parseShader(const std::string &filePath);
unsigned int compileShader(unsigned int type, const std::string &source);
unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
void createFullScreen();

#endif
