#include "CameraController.h"

CameraController::CameraController(PerspectiveCamera& camera, float windowWidth, float windowHeight) : camera(camera), windowWidth(windowWidth), windowHeight(windowHeight), lastX(0), lastY(0) {}

void CameraController::mouseCallback(double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.001f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    float angleY = xoffset;
    float angleX = yoffset;

    camera.rotate(-angleY, camera.getUp());
    camera.rotate(angleX, camera.getRight());
}

void CameraController::keyCallback(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        Vec3<float> movement(0.0f, 0.0f, 0.0f);

        float speed = 10.0f;

        if (key == GLFW_KEY_W) movement = movement + camera.getForward() * speed;
        if (key == GLFW_KEY_S) movement = movement - camera.getForward() * speed;
        if (key == GLFW_KEY_A) movement = movement - camera.getRight() * speed;
        if (key == GLFW_KEY_D) movement = movement + camera.getRight() * speed;

        camera.translate(movement);
    }
}

void CameraController::onMouseMove(const Event& event) {
    const MouseMoveEvent& mouseEvent = static_cast<const MouseMoveEvent&>(event);
    mouseCallback(mouseEvent.xpos, mouseEvent.ypos);
}

void CameraController::onKeyPress(const Event& event) {
    const KeyEvent& keyEvent = static_cast<const KeyEvent&>(event);
    keyCallback(keyEvent.key, keyEvent.scancode, keyEvent.action, keyEvent.mods);
}
