#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

bool initGLAD();

//called by GLFW whenever an error occurs within the library
void errorCallback(int error, const char* description);
bool initGLFW();
void setWindowHints();
void setupOpenGL();

//called whenever the window is resized
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

void checkGLError(const std::string& label);