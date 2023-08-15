#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float near, float far)
    : fov(fov), aspectRatio(aspectRatio), near(near), far(far) {
    updateProjectionMatrix();
}

void PerspectiveCamera::updateViewMatrix() {
    // Assuming the right vector is derived from forward and up
    right = forward.cross(up).normalize();

    // Compute the view matrix based on position, forward, up, and right vectors
    viewMatrix = Mat4<float>{
        right.x(), right.y(), right.z(), -right.dot(position),
        up.x(), up.y(), up.z(), -up.dot(position),
        -forward.x(), -forward.y(), -forward.z(), forward.dot(position),
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

Mat4<float> PerspectiveCamera::getViewMatrix() const {
    return viewMatrix;
}

void PerspectiveCamera::updateProjectionMatrix() {
    // Compute the projection matrix based on fov, aspect ratio, near and far clipping planes
    float tanHalfFov = std::tan(fov / 2.0f);
    float range = near - far;

    projectionMatrix = Mat4<float>{
        1.0f / (aspectRatio * tanHalfFov), 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f / tanHalfFov, 0.0f, 0.0f,
        0.0f, 0.0f, (near + far) / range, 2.0f * near * far / range,
        0.0f, 0.0f, -1.0f, 0.0f
    };
}

Mat4<float> PerspectiveCamera::getProjectionMatrix() const {
    return projectionMatrix;
}
