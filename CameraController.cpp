#include "CameraController.h"

CameraController::CameraController(PerspectiveCamera& camera, float windowWidth, float windowHeight) : camera(camera), windowWidth(windowWidth), windowHeight(windowHeight), lastX(0), lastY(0) {}

void CameraController::mouseCallback(double xpos, double ypos) {

    // Convertendo as coordenadas do mouse para coordenadas na tela
    float x = (2.0f * xpos) / windowWidth - 1.0f;
    float y = 1.0f - (2.0f * ypos) / windowHeight;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    // Convertendo os offsets em um ângulo de rotação (em graus)
    float angle = std::sqrt(xoffset * xoffset + yoffset * yoffset);

    // Fator de escala para suavizar a transição
    float scaleFactor = 0.01f;

    angle *= scaleFactor; // Multiplica o ângulo pelo fator de escala

    // Calculando o vetor direcional baseado nas coordenadas na tela
    Vec3<float> direction;
    direction.x() = x * cos(angle) - y * sin(angle);
    direction.y() = y * cos(angle) + x * sin(angle);
    direction.z() = -1.0f; // a câmera está olhando na direção negativa do eixo Z
    direction = direction.normalize(); // normalize o vetor

    // Calculando o ponto para o qual a câmera deve olhar
    Vec3<float> lookAtPoint = camera.getPosition() + direction;

    // Configurando a câmera para olhar para esse ponto
    camera.lookAt(lookAtPoint);
}

void CameraController::keyCallback(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        Vec3<float> movement(0.0f, 0.0f, 0.0f);

        float speed = 0.1f;

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
