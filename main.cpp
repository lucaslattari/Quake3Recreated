#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "bsp.h"

using namespace std;

//called by GLFW whenever an error occurs within the library
void errorCallback(int error, const char* description) {
    cout << "GLFW Error (" << error << "): " << description << endl;
}

//called whenever the window is resized
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

bool initGLFW() {
    if (!glfwInit()) {
        cout << "Failed to initialize GLFW." << endl;
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

bool initGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD." << endl;
        return false;
    }
    return true;
}

void setupOpenGL() {
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // Set the background color
    glEnable(GL_DEPTH_TEST); // Enable depth testing (for 3D rendering)
}

void renderFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffers

    // Add your rendering or updating code here
}

int main(int argc, char* argv[]) {
    if (!initGLFW()) {
        return -1;
    }

    setWindowHints();

    Quake3BSPLoader q3bsp_map;
    if (!q3bsp_map.load("maps/render.bsp")) {
        std::cerr << "Error loading BSP file" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "CEREBRO ENGINE", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window." << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!initGLAD()) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    setupOpenGL();

    while (!glfwWindowShouldClose(window)) {
        renderFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
