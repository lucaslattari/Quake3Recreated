#pragma once

#include "camera.h"
#include <cmath>

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(float fov, float aspectRatio, float near, float far);

    void updateViewMatrix() override;
    Mat4<float> getViewMatrix() const override;

    void updateProjectionMatrix() override;
    Mat4<float> getProjectionMatrix() const override;

    void setFov(float newFov);

private:
    float fov;
    float aspectRatio;
    float near;
    float far;
    Mat4<float> viewMatrix;
    Mat4<float> projectionMatrix;
};
