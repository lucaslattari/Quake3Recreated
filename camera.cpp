#include "camera.h"

// Getters
Vec3<float> Camera::getPosition() const {
    return position;
}

Vec3<float> Camera::getForward() const {
    return forward;
}

Vec3<float> Camera::getUp() const {
    return up;
}

Vec3<float> Camera::getRight() const {
    return right;
}

// Setters
void Camera::setPosition(const Vec3<float>& newPosition) {
    position = newPosition;
}

void Camera::setForward(const Vec3<float>& newForward) { // Implementado
    forward = newForward;
}

void Camera::setUp(const Vec3<float>& newUp) { // Implementado
    up = newUp;
}

void Camera::setRight(const Vec3<float>& newRight) { // Implementado
    right = newRight;
}

// Translation and rotation
void Camera::translate(const Vec3<float>& translation) {
    position = position + translation;
}

void Camera::rotate(float angle, const Vec3<float>& axis) {
    // Apply the rotation to the forward and up vectors
    Mat4<float> rotationMatrix = Mat4<float>::rotate(angle, axis);
    Vec4<float> newForward4 = rotationMatrix * Vec4<float>{forward.x(), forward.y(), forward.z(), 0.0f};
    Vec4<float> newUp4 = rotationMatrix * Vec4<float>{up.x(), up.y(), up.z(), 0.0f};

    // Update the forward and up vectors with the rotated values
    forward = Vec3<float>{ newForward4.x(), newForward4.y(), newForward4.z() };
    up = Vec3<float>{ newUp4.x(), newUp4.y(), newUp4.z() };

    // Update the right vector
    right = forward.cross(up).normalize();
}

void Camera::lookAt(const Vec3<float>& target) {
    forward = (target - position).normalize();
    up = Vec3f(0.0f, 1.0f, 0.0f);
    right = forward.cross(up).normalize();
    up = right.cross(forward).normalize();

    updateViewMatrix();
}
