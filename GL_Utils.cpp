#include <iostream>

#include "GL_Utils.h"

bool initGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD." << std::endl;
        return false;
    }
    return true;
}

//called by GLFW whenever an error occurs within the library
void errorCallback(int error, const char* description) {
    std::cout << "GLFW Error (" << error << "): " << description << std::endl;
}

bool initGLFW() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW." << std::endl;
        return false;
    }

    glfwSetErrorCallback(errorCallback);
    return true;
}

void setWindowHints() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void setupOpenGL() {
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // Set the background color
    glEnable(GL_DEPTH_TEST); // Enable depth testing (for 3D rendering)
}

//called whenever the window is resized
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}