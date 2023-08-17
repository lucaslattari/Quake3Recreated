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
    forward = Vec3<float>{ newForward4.x(), newForward4.y(), newForward4.z()};
    up = Vec3<float>{ newUp4.x(), newUp4.y(), newUp4.z()};

    // Update the right vector
    right = forward.cross(up).normalize();
}

void Camera::lookAt(const Vec3<float>& target) {
    // Calcular o novo vetor direcional (forward)
    forward = (target - position).normalize();

    // Recalcular o vetor para cima (up)
    up = Vec3f(0.0f, 1.0f, 0.0f);

    // Calcular o vetor à direita (right)
    right = forward.cross(up).normalize();

    // Normalizar o vetor para cima (up)
    up = right.cross(forward).normalize(); // note a ordem invertida para manter a mão direita

    // (Opcional) Atualizar a matriz de visualização se necessário
    updateViewMatrix();
}