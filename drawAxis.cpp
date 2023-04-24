#pragma once

#include "drawAxis.h"

AxisDrawer::AxisDrawer() {
    initShader();
    initBuffers();
}

AxisDrawer::~AxisDrawer() {
    glDeleteProgram(shaderProgram);
    glDeleteVertexArrays(1, &axis_vao);
    glDeleteBuffers(1, &axis_vbo);
    glDeleteBuffers(1, &color_vbo);
}

void AxisDrawer::draw() {
    glUseProgram(shaderProgram); //loads the program containing the two compiled shaders into the OpenGL pipeline stages
    glBindVertexArray(axis_vao);
    glDrawArrays(GL_LINES, 0, 6);
    glBindVertexArray(0);
}

void AxisDrawer::initShader() {
    // Vertex shader source code
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "out vec3 color;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos, 1.0);\n"
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
    shaderProgram = glCreateProgram();
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
}

void AxisDrawer::initBuffers() {
    // Define axis vertices and colors
    GLfloat x_axis_vertices[] = { -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
    GLfloat y_axis_vertices[] = { 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
    GLfloat z_axis_vertices[] = { 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f };

    GLfloat x_axis_color[] = { 1.0f, 0.0f, 0.0f };
    GLfloat y_axis_color[] = { 0.0f, 1.0f, 0.0f };
    GLfloat z_axis_color[] = { 0.0f, 0.0f, 1.0f };

    // Generate and bind a vertex array object (VAO) for the axis
    // bind = make it "active"
    glGenVertexArrays(1, &axis_vao);
    glBindVertexArray(axis_vao);

    // Generate and bind a vertex buffer object (VBO) for the axis vertices
    glGenBuffers(1, &axis_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, axis_vbo);

    // Copy the axis vertices into the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(x_axis_vertices) + sizeof(y_axis_vertices) + sizeof(z_axis_vertices), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(x_axis_vertices), x_axis_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(x_axis_vertices), sizeof(y_axis_vertices), y_axis_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(x_axis_vertices) + sizeof(y_axis_vertices), sizeof(z_axis_vertices), z_axis_vertices);

    // Specify the layout of the vertex data in the VBO

    /*
    * glVertexAttribPointer: define how the vertex data should be read from the VBO
    * 0: index of vertex attribute (corresponds to the aPos in vertex shader)
    * 3: number of componentes per vertex attribute (x, y and z)
    * GL_FLOAT: components are float values
    * GL_FALSE: no normalization in data will be performed
    * 0: offset between consecutive vertex attributes (strides)
    * (void*)0: byte offset of first vertex attribute
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); //associate attribute with buffer
    glEnableVertexAttribArray(0); //enable the vertex attribute array

    // Generate and bind a vertex buffer object (VBO) for the axis colors
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

    // Unbind the VAO and VBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}