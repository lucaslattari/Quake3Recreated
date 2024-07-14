#include "CameraController.h"
#include "math_utils.h"  // Incluir o arquivo de utilidades matemáticas

CameraController::CameraController(PerspectiveCamera& camera, float windowWidth, float windowHeight)
    : camera(camera), windowWidth(windowWidth), windowHeight(windowHeight), lastX(0), lastY(0) {}

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

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Limitar pitch para evitar que a câmera vire de cabeça para baixo
    pitch = std::max(-89.0f, std::min(89.0f, pitch));

    // Atualizar a direção da câmera
    Vec3f direction;
    direction.x() = cos(radians(yaw)) * cos(radians(pitch));
    direction.y() = sin(radians(pitch));
    direction.z() = sin(radians(yaw)) * cos(radians(pitch));
    camera.setForward(direction.normalize());
    camera.setRight(camera.getForward().cross(Vec3f(0.0f, 1.0f, 0.0f)).normalize());
    camera.setUp(camera.getRight().cross(camera.getForward()).normalize());
}

void CameraController::scrollCallback(double xoffset, double yoffset) {
    zoom -= yoffset;
    zoom = std::max(1.0f, std::min(45.0f, zoom));
    camera.setFov(zoom);
}

void CameraController::keyCallback(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        Vec3f movement(0.0f, 0.0f, 0.0f);
        float currentForwardSpeed = forwardSpeed;
        float currentSideSpeed = sideSpeed;

        // Aplicar boost se a tecla Shift estiver pressionada
        if (mods & GLFW_MOD_SHIFT) {
            currentForwardSpeed *= boostMultiplier;
            currentSideSpeed *= boostMultiplier;
        }

        if (key == GLFW_KEY_W) movement = movement + camera.getForward() * currentForwardSpeed;
        if (key == GLFW_KEY_S) movement = movement - camera.getForward() * currentForwardSpeed;
        if (key == GLFW_KEY_A) movement = movement - camera.getRight() * currentSideSpeed;
        if (key == GLFW_KEY_D) movement = movement + camera.getRight() * currentSideSpeed;

        camera.translate(movement);
    }
}

void CameraController::onMouseMove(const Event& event) {
    const MouseMoveEvent& mouseEvent = static_cast<const MouseMoveEvent&>(event);
    mouseCallback(mouseEvent.xpos, mouseEvent.ypos);
}

void CameraController::onMouseScroll(const Event& event) {
    const MouseScrollEvent& scrollEvent = static_cast<const MouseScrollEvent&>(event);
    scrollCallback(scrollEvent.xoffset, scrollEvent.yoffset);
}

void CameraController::onKeyPress(const Event& event) {
    const KeyEvent& keyEvent = static_cast<const KeyEvent&>(event);
    keyCallback(keyEvent.key, keyEvent.scancode, keyEvent.action, keyEvent.mods);
}

void CameraController::setMouseSensitivity(float sensitivity) {
    this->sensitivity = sensitivity;
}

void CameraController::setMovementSpeed(float forwardSpeed, float sideSpeed) { // Modificado
    this->forwardSpeed = forwardSpeed;
    this->sideSpeed = sideSpeed;
}

void CameraController::setBoostMultiplier(float boost) {
    this->boostMultiplier = boost;
}
