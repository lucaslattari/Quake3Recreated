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
    Vec3<float> getUp() const;
    Vec3<float> getRight() const;
    void setOrientation(const Vec3<float>& newForward, const Vec3<float>& newUp);

    // Common methods for translation and rotation
    void translate(const Vec3<float>& translation);
    void rotate(float angle, const Vec3<float>& axis);

protected:
    // Protected attributes for position and orientation
    Vec3<float> position;
    Vec3<float> forward;
    Vec3<float> up;
    Vec3<float> right;
};