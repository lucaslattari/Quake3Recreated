#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class AxisDrawer {
public:
    AxisDrawer();
    ~AxisDrawer();
    void draw();

private:
    GLuint shaderProgram;
    GLuint axis_vao;
    GLuint axis_vbo;
    GLuint color_vbo;

    void initShader();
    void initBuffers();
};