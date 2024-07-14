#pragma once

#include "vector.h"
#include "matrix.h"

class Camera {
public:
    virtual ~Camera() = default;

    virtual void updateViewMatrix() = 0;
    virtual Mat4<float> getViewMatrix() const = 0;

    virtual void updateProjectionMatrix() = 0;
    virtual Mat4<float> getProjectionMatrix() const = 0;

    // Common getter and setter methods for position and orientation
    Vec3<float> getPosition() const;
    void setPosition(const Vec3<float>& newPosition);
    Vec3<float> getForward() const;
    void setForward(const Vec3<float>& newForward); // Adicionado
    Vec3<float> getUp() const;
    void setUp(const Vec3<float>& newUp); // Adicionado
    Vec3<float> getRight() const;
    void setRight(const Vec3<float>& newRight); // Adicionado

    // Common methods
    void translate(const Vec3<float>& translation);
    void rotate(float angle, const Vec3<float>& axis);
    void lookAt(const Vec3<float>& target);

protected:
    // Protected attributes for position and orientation
    Vec3<float> position;
    Vec3<float> forward;
    Vec3<float> up;
    Vec3<float> right;
};
