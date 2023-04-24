#include <iostream>
#include <sstream> // Include stringstream
#include <iomanip> // Include setprecision
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "drawAxis.h"
#include "bsp.h"

using namespace std;

void printFPS(GLFWwindow* window) {
    // Declare static variables to store the previous time and frame count
    static double previousTime = glfwGetTime();
    static int frameCount = 0;

    // Increment the frame count
    frameCount++;

    // Get the current time
    double currentTime = glfwGetTime();

    // Calculate the elapsed time since the previous measurement
    double elapsedTime = currentTime - previousTime;

    // Update and print FPS every second (1.0)
    if (elapsedTime >= 1.0) {
        double fps = static_cast<double>(frameCount) / elapsedTime;

        // Create a string stream to format the FPS value
        std::stringstream titleStream;
        titleStream << std::fixed << std::setprecision(2) << "CEREBRO ENGINE - FPS: " << fps;

        // Set the window title with the new FPS value
        glfwSetWindowTitle(window, titleStream.str().c_str());

        // Reset the frame count and update the previous time
        frameCount = 0;
        previousTime = currentTime;
    }
}

//called by GLFW whenever an error occurs within the library
void errorCallback(int error, const char* description) {
    cout << "GLFW Error (" << error << "): " << description << endl;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
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

void renderFrame(AxisDrawer& axisDrawer) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffers

    // Add rendering or updating code here

    axisDrawer.draw(); // Call the draw method of AxisDrawer
}

int main(int argc, char* argv[]) {
    if (!initGLFW()) {
        return -1;
    }

    setWindowHints();

    BSP::Loader q3bsp_map;
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

    // Register the key callback function
    glfwSetKeyCallback(window, keyCallback);

    if (!initGLAD()) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    setupOpenGL();

    AxisDrawer axisDrawer; // object that draw Axis

    while (!glfwWindowShouldClose(window)) {
        renderFrame(axisDrawer);

        printFPS(window); // Calculate and print the FPS in the window title

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
