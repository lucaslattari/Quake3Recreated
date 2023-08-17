#pragma once

#include <GLFW/glfw3.h>
#include <string>

void warning_assert(bool condition, const std::string& message);
void progressBar(float progress);
void printFPS(GLFWwindow* window);