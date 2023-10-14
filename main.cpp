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

int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;

// Function to set up the camera
PerspectiveCamera setupCamera() {
    PerspectiveCamera camera(60, WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 10000.0f);
    Vec3f cameraPosition = Vec3f(0.0f, 0.0f, 100.0f); // Camera position
    camera.setPosition(cameraPosition);

    camera.lookAt(Vec3f(0.0f, 0.0f, 0.0f)); // Point the camera is looking at
    camera.updateProjectionMatrix();

    return camera;
}

// Function to render a frame
void renderFrame(PerspectiveCamera& camera, GLuint shaderProgram, BSP::Loader& map) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen

    camera.updateViewMatrix();
    camera.updateProjectionMatrix();

    Mat4<float> modelMatrix = Mat4<float>{}; // Use identity matrix or set your desired model matrix here
    Mat4<float> viewMatrix = camera.getViewMatrix().transpose();
    Mat4<float> projectionMatrix = camera.getProjectionMatrix().transpose();

    glUseProgram(shaderProgram); // Use the shader program
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, modelMatrix.getData());
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, viewMatrix.getData());
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, projectionMatrix.getData());

    drawAxes(shaderProgram); // Draw coordinate axes

    map.drawLevel(Vec3<float>(0.0f, 0.0f, 0.0f), shaderProgram);
}

int main(int argc, char* argv[]) {
    if (!initGLFW()) {
        return -1; // GLFW initialization failed
    }

    setWindowHints(); // Set OpenGL window hints

    // Create GLFW window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CEREBRO ENGINE", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window." << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Capture the mouse cursor
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!initGLAD()) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    setupOpenGL();
    GLuint shaderProgram = initShader(); // Initialize shader

    PerspectiveCamera camera = setupCamera(); // Setup camera
    CameraController cameraController(camera, WINDOW_WIDTH, WINDOW_HEIGHT); // Create camera controller

    BSP::Loader BSPMap; // Load BSP map
    if (!BSPMap.load("maps/render.bsp")) {
        std::cerr << "Error loading BSP file" << std::endl;
        return -1;
    }

    EventSystem eventSystem;
    // Add listeners for key press and mouse move events
    eventSystem.addListener(EventType::KeyPress, std::bind(&CameraController::onKeyPress, &cameraController, std::placeholders::_1));
    eventSystem.addListener(EventType::MouseMove, std::bind(&CameraController::onMouseMove, &cameraController, std::placeholders::_1));

    // Wireframe mode
    bool wireframeMode = false;
    eventSystem.addListener(EventType::KeyPress, [&wireframeMode](const Event& event) {
        const KeyEvent& keyEvent = static_cast<const KeyEvent&>(event);
        if (keyEvent.action == GLFW_PRESS && keyEvent.key == GLFW_KEY_1) {
            wireframeMode = !wireframeMode;  // Toggle wireframe mode
            if (wireframeMode) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
        }
        });

    // Set callbacks for GLFW window
    glfwSetWindowUserPointer(window, &eventSystem);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);

    // Add listener for escape key or 'q'
    eventSystem.addListener(EventType::KeyPress, [&window](const Event& event) {
        const KeyEvent& keyEvent = static_cast<const KeyEvent&>(event);
        if (keyEvent.action == GLFW_PRESS && (keyEvent.key == GLFW_KEY_ESCAPE || keyEvent.key == GLFW_KEY_Q)) {
            glfwSetWindowShouldClose(window, true);
        }
        });

    // Listener for F1, F2, F3 keys to toggle rendering modes
    eventSystem.addListener(EventType::KeyPress, [&BSPMap](const Event& event) {
        const KeyEvent& keyEvent = static_cast<const KeyEvent&>(event);
        if (keyEvent.action == GLFW_PRESS) {
            switch (keyEvent.key) {
            case GLFW_KEY_F1:
                BSPMap.setRenderPolygonsAndMeshes(true);
                BSPMap.setRenderPatches(false);
                break;
            case GLFW_KEY_F2:
                BSPMap.setRenderPolygonsAndMeshes(false);
                BSPMap.setRenderPatches(true);
                break;
            case GLFW_KEY_F3:
                BSPMap.setRenderPolygonsAndMeshes(true);
                BSPMap.setRenderPatches(true);
                break;
            default:
                break;
            }
        }
        });

    while (!glfwWindowShouldClose(window)) {
        renderFrame(camera, shaderProgram, BSPMap); // Render the frame

        printFPS(window); // Calculate and print the FPS in the window title

        glfwSwapBuffers(window);
        glfwPollEvents(); // Poll for and process events
    }

    glfwDestroyWindow(window);
    glfwTerminate(); // Terminate GLFW
    return 0;
}
