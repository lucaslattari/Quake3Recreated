#include <cmath>

#include "GL_Utils.h"
#include "utils.h"

#include "EventSystem.h"
#include "PerspectiveCamera.h"
#include "CameraController.h"
#include "BasicShapes.h"
#include "shaders.h"
#include "bsp.h"

using namespace std;

int WINDOW_WIDTH = 1280.0f;
int WINDOW_HEIGHT = 720.0f;

PerspectiveCamera setupCamera(){
    PerspectiveCamera camera(60, WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 1000.0f);
    Vec3f cameraPosition = Vec3f(0.0f, 0.0f, 3.0f); // Posição da câmera
    camera.setPosition(cameraPosition);

    camera.lookAt(Vec3f(0.0f, 0.0f, 0.0f));

    camera.updateProjectionMatrix();

    return camera;
}

void renderFrame(PerspectiveCamera& camera, GLuint shaderProgram) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.updateViewMatrix();
    camera.updateProjectionMatrix();

    Mat4<float> modelMatrix = Mat4<float>{}; // Use identity matrix or set your desired model matrix here
    Mat4<float> viewMatrix = camera.getViewMatrix().transpose();
    Mat4<float> projectionMatrix = camera.getProjectionMatrix().transpose();

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, modelMatrix.getData());
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, viewMatrix.getData());
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, projectionMatrix.getData());

    drawAxes(shaderProgram);
    drawTriangle(shaderProgram);
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

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CEREBRO ENGINE", NULL, NULL);
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
    GLuint shaderProgram = initShader(); // Função para inicializar o shader

    PerspectiveCamera camera = setupCamera();
    CameraController cameraController(camera, WINDOW_WIDTH, WINDOW_HEIGHT);

    EventSystem eventSystem;
    eventSystem.addListener(EventType::KeyPress, std::bind(&CameraController::onKeyPress, &cameraController, std::placeholders::_1));
    eventSystem.addListener(EventType::MouseMove, std::bind(&CameraController::onMouseMove, &cameraController, std::placeholders::_1));

    glfwSetWindowUserPointer(window, &eventSystem);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);

    // Adicionar um novo ouvinte para a tecla ESC ou 'q'
    eventSystem.addListener(EventType::KeyPress, [&window](const Event& event) {
        const KeyEvent& keyEvent = static_cast<const KeyEvent&>(event);
        if (keyEvent.action == GLFW_PRESS && (keyEvent.key == GLFW_KEY_ESCAPE || keyEvent.key == GLFW_KEY_Q)) {
            glfwSetWindowShouldClose(window, true);
        }
    });

    while (!glfwWindowShouldClose(window)) {
        renderFrame(camera, shaderProgram);

        printFPS(window); // Calculate and print the FPS in the window title

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}