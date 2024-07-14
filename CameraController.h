#pragma once

#include "GL_Utils.h"
#include "EventSystem.h"
#include "PerspectiveCamera.h"

class CameraController {
private:
    float lastX, lastY, windowWidth, windowHeight;
    bool firstMouse = true;
    PerspectiveCamera& camera;
    float pitch = 0.0f;
    float yaw = 0.0f;
    float forwardSpeed = 10.0f; // Velocidade de movimento para frente
    float sideSpeed = 10.0f; // Velocidade de movimento lateral
    float sensitivity = 0.01f;
    float zoom = 45.0f;
    float boostMultiplier = 2.0f; // Speed boost multiplier

public:
    CameraController(PerspectiveCamera& camera, float windowWidth, float windowHeight);

    void mouseCallback(double xpos, double ypos);
    void scrollCallback(double xoffset, double yoffset);
    void keyCallback(int key, int scancode, int action, int mods);

    void onMouseMove(const Event& event);
    void onMouseScroll(const Event& event);
    void onKeyPress(const Event& event);

    void setMouseSensitivity(float sensitivity);
    void setMovementSpeed(float forwardSpeed, float sideSpeed); // Modificado
    void setBoostMultiplier(float boost);
};
