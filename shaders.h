#pragma once

#include "GL_Utils.h"

GLuint initShader() {
    // Vertex shader source code
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "out vec3 color;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
        "   color = aColor;\n"
        "}\0";
    

    // Fragment shader source code
    const char* fragmentShaderSource = "#version 330 core\n"
        "in vec3 color;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(color, 1.0);\n"
        "}\n\0";

    // Compile and link shader program
    int success;
    char infoLog[512];

    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    /*
    * glShadersource: loads the GLSL code from the strings into the empty shader objects
    *
    * vertexShader: the shader object in which to store the shader
    * 1: number of strings in the shader source code
    * vertexShaderSource: an array of pointers to strings containing the source code
    */
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    /*
    * glGetshaderiv: retrieves the compile status of the specified shader
    * (in this case, vertexShader). The GL_COMPILE_STATUS parameter tells the
    * function to get the compilation status. The result is stored in the success
    * variable. If the compilation was successful, success will be set to a non-zero
    * value (usually GL_TRUE), otherwise, it will be set to zero (usually GL_FALSE).
    *
    */
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        //retrieves the error log. 512 is the max length of log message stored in infolog
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Vertex shader compilation failed: " << infoLog << std::endl;
    }

    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Fragment shader compilation failed: " << infoLog << std::endl;
    }

    // Link shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader program linking failed: " << infoLog << std::endl;
    }

    // Clean up shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

