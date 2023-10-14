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

void drawTriangle(GLuint shaderProgram, Vec3f v1, Vec3f v2, Vec3f v3) {
    // Set up vertex data for the triangle
    float vertices[] = {
        v1.x(), v1.y(), v1.z(), 0.0f, 0.0f, 1.0f,
        v2.x(), v2.y(), v2.z(), 0.0f, 0.0f, 1.0f,
        v3.x(), v3.y(), v3.z(), 0.0f, 0.0f, 1.0f,
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

//muito ineficiente, só usar para fins de depuração rápida
void drawSphere(GLuint shaderProgram, float radius, Vec3f p) {
    const unsigned int sectors = 36;  // setores horizontais
    const unsigned int stacks = 18;  // pilhas verticais

    std::vector<float> sphereVertices;

    // Geração dos vértices da esfera
    for (unsigned int i = 0; i <= stacks; ++i) {
        float theta = i * 3.14 / stacks;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (unsigned int j = 0; j <= sectors; ++j) {
            float phi = j * 2 * 3.14 / sectors;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            float x = cosPhi * sinTheta;
            float y = cosTheta;
            float z = sinPhi * sinTheta;

            sphereVertices.push_back(p.x() + radius * x);
            sphereVertices.push_back(p.y() + radius * y);
            sphereVertices.push_back(p.z() + radius * z);
            sphereVertices.push_back(1.0f);  // Cor vermelha
            sphereVertices.push_back(0.0f);
            sphereVertices.push_back(0.0f);
        }
    }

    // Geração dos índices
    std::vector<GLuint> sphereIndices;
    for (unsigned int i = 0; i < stacks; ++i) {
        unsigned int k1 = i * (sectors + 1);
        unsigned int k2 = k1 + sectors + 1;

        for (unsigned int j = 0; j < sectors; ++j, ++k1, ++k2) {
            if (i != 0) {
                sphereIndices.push_back(k1);
                sphereIndices.push_back(k2);
                sphereIndices.push_back(k1 + 1);
            }

            if (i != (stacks - 1)) {
                sphereIndices.push_back(k1 + 1);
                sphereIndices.push_back(k2);
                sphereIndices.push_back(k2 + 1);
            }
        }
    }

    GLuint sphereVao, sphereVbo, sphereEbo;
    glGenVertexArrays(1, &sphereVao);
    glGenBuffers(1, &sphereVbo);
    glGenBuffers(1, &sphereEbo);

    glBindVertexArray(sphereVao);

    glBindBuffer(GL_ARRAY_BUFFER, sphereVbo);
    glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), sphereVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(GLuint), sphereIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Desenha a esfera
    glUseProgram(shaderProgram);
    glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);

    // Limpeza
    glDeleteVertexArrays(1, &sphereVao);
    glDeleteBuffers(1, &sphereVbo);
    glDeleteBuffers(1, &sphereEbo);
}