#include <iostream>
#include <iomanip> // Include setprecision
#include <sstream> // Include stringstream
#include <chrono>
#include <thread>

#include "utils.h"

void warning_assert(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "Assertion failed: " << message << std::endl;
    }
}

void progressBar(float progress) {
    const int barWidth = 70;
    std::cout << "[";
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
}

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