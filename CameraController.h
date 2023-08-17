#pragma once

#include "GL_Utils.h"

#include "EventSystem.h"
#include "PerspectiveCamera.h"

class CameraController {
private:
    float lastX, lastY, windowWidth, windowHeight;
    bool firstMouse = true;
    PerspectiveCamera& camera;

public:
    CameraController(PerspectiveCamera& camera, float windowWidth, float windowHeight);

    void mouseCallback(double xpos, double ypos);
    void keyCallback(int key, int scancode, int action, int mods);

    void onMouseMove(const Event& event);
    void onKeyPress(const Event& event);
};
