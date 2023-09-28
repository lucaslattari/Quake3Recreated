#include "BasicShapes.h"

void drawAxes(GLuint shaderProgram) {
    float axesVertices[] = {
        // Eixo X (vermelho)
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        // Eixo Y (verde)
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 100.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        // Eixo Z (azul)
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 100.0f, 0.0f, 0.0f, 1.0f
    };

    GLuint axesVao, axesVbo;
    glGenVertexArrays(1, &axesVao);
    glGenBuffers(1, &axesVbo);

    glBindVertexArray(axesVao);
    glBindBuffer(GL_ARRAY_BUFFER, axesVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axesVertices), axesVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Desenha os eixos
    glUseProgram(shaderProgram);
    glDrawArrays(GL_LINES, 0, 6);

    // Limpa
    glDeleteVertexArrays(1, &axesVao);
    glDeleteBuffers(1, &axesVbo);
}

void drawTriangle(GLuint shaderProgram) {
    // Set up vertex data for the triangle
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    };

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Draw the triangle
    glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 0.0f, 0.0f, 1.0f); // Azul
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Clean up VBO and VAO
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

}
