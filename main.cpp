#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "bsp.h"

using namespace std;

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

void drawAxes() {
    // Define the x-axis vertices
    float x_axis_vertices[] = {
        -1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f
    };

    // Define the y-axis vertices
    float y_axis_vertices[] = {
        0.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    // Define the z-axis vertices
    float z_axis_vertices[] = {
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, 1.0f
    };

    // Define the color for each axis
    float x_axis_color[] = { 1.0f, 0.0f, 0.0f }; // Red
    float y_axis_color[] = { 0.0f, 1.0f, 0.0f }; // Green
    float z_axis_color[] = { 0.0f, 0.0f, 1.0f }; // Blue

    // Compile the shader program
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "out vec3 color;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos, 1.0);\n"
        "   color = aColor;\n"
        "}\0";
    const char* fragmentShaderSource = "#version 330 core\n"
        "in vec3 color;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(color, 1.0);\n"
        "}\n\0";

    //TODO: estudar melhor o que é feito aqui
    int success;
    char infoLog[512];
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Vertex shader compilation failed: " << infoLog << std::endl;
    }
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Fragment shader compilation failed: " << infoLog << std::endl;
    }
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram

    (shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader program linking failed: " << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Generate and bind a vertex array object (VAO) for the axes
    unsigned int axis_vao;
    glGenVertexArrays(1, &axis_vao);
    glBindVertexArray(axis_vao);

    // Generate and bind a vertex buffer object (VBO) for the axis vertices
    unsigned int axis_vbo;
    glGenBuffers(1, &axis_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, axis_vbo);

    // Copy the axis vertices into the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(x_axis_vertices) + sizeof(y_axis_vertices) + sizeof(z_axis_vertices), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(x_axis_vertices), x_axis_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(x_axis_vertices), sizeof(y_axis_vertices), y_axis_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(x_axis_vertices) + sizeof(y_axis_vertices), sizeof(z_axis_vertices), z_axis_vertices);

    // Specify the layout of the vertex data in the VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // Generate and bind a vertex buffer object (VBO) for the axis colors
    unsigned int color_vbo;
    glGenBuffers(1, &color_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);

    // Copy the axis colors into the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(x_axis_color) + sizeof(y_axis_color) + sizeof(z_axis_color), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(x_axis_color), x_axis_color);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(x_axis_color), sizeof(y_axis_color), y_axis_color);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(x_axis_color) + sizeof(y_axis_color), sizeof(z_axis_color), z_axis_color);

    // Specify the layout of the color data in the VBO
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    // Draw the axes
    glUseProgram(shaderProgram);
    glDrawArrays(GL_LINES, 0, 6);

    // Unbind the VAO and VBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void renderFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color and depth buffers

    // Add your rendering or updating code here
    drawAxes();
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

    while (!glfwWindowShouldClose(window)) {
        renderFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
