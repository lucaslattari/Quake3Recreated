#pragma once

#include "camera.h"

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(float fov, float aspectRatio, float near, float far);

    void updateViewMatrix() override;
    Matrix<float, 4, 4> getViewMatrix() const override;

    // Perspective-specific attributes and methods can be included here.
};