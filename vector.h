#pragma once

#include <array>
#include <cmath>
#include <algorithm>
#include <cassert>
#include <numeric>
#include <ostream>

template <typename T, int N>
class Vector {
protected:
    std::array<T, N> data;

public:
    // Accessors
    T& operator[](int index) { return data[index]; }
    const T& operator[](int index) const { return data[index]; }
    Vector(const Vector& other) = default;
    Vector& operator=(const Vector& other) = default;

    // Constructors
    Vector() : data{} {}
    Vector(const T* values) { std::copy_n(values, N, data.begin()); }
    Vector(std::initializer_list<T> values) {
        assert(values.size() <= N);
        std::copy(values.begin(), values.end(), data.begin());
    }

    // Arithmetic operations
    Vector operator+(const Vector& other) const {
        Vector result;
        std::transform(data.begin(), data.end(), other.data.begin(), result.data.begin(), std::plus<T>());
        return result;
    }

    Vector operator-(const Vector& other) const {
        Vector result;
        std::transform(data.begin(), data.end(), other.data.begin(), result.data.begin(), std::minus<T>());
        return result;
    }

    Vector operator*(T scalar) const {
        Vector result;
        std::transform(data.begin(), data.end(), result.data.begin(), [scalar](T val) { return val * scalar; });
        return result;
    }

    // Dot product
    T dot(const Vector& other) const {
        return std::inner_product(data.begin(), data.end(), other.data.begin(), T(0));
    }

    // Length and normalization
    T length() const {
        return std::sqrt(std::inner_product(data.begin(), data.end(), data.begin(), T(0)));
    }

    Vector normalize() const {
        T len = length();
        Vector result;
        std::transform(data.begin(), data.end(), result.data.begin(), [len](T value) { return value / len; });
        return result;
    }
};

template <typename T, int N>
std::ostream& operator<<(std::ostream& os, const Vector<T, N>& vec) {
    os << "(";
    for (int i = 0; i < N; ++i) {
        os << vec[i];
        if (i < N - 1) os << ", ";
    }
    os << ")";
    return os;
}

// Specializations for common 2D, 3D, and 4D vector types
template<typename T>
class Vec2 : public Vector<T, 2> {
public:
    Vec2() : Vector<T, 2>() {}

    Vec2(T x, T y) : Vector<T, 2>({ x, y }) {}

    T& x() { return this->data[0]; }
    const T& x() const { return this->data[0]; }

    T& y() { return this->data[1]; }
    const T& y() const { return this->data[1]; }

    Vec2(const Vector<T, 2>& other) : Vector<T, 2>(other) {}
};

template<typename T>
class Vec3 : public Vector<T, 3> {
public:
    Vec3() : Vector<T, 3>() {}
    Vec3(T x, T y, T z) : Vector<T, 3>({ x, y, z }) {}

    T& x() { return this->data[0]; }
    const T& x() const { return this->data[0]; }

    T& y() { return this->data[1]; }
    const T& y() const { return this->data[1]; }

    T& z() { return this->data[2]; }
    const T& z() const { return this->data[2]; }

    Vec3(const Vector<T, 3>& other) : Vector<T, 3>(other) {}

    Vec3 cross(const Vec3& other) const {
        return Vec3{
            this->data[1] * other.data[2] - this->data[2] * other.data[1],
            this->data[2] * other.data[0] - this->data[0] * other.data[2],
            this->data[0] * other.data[1] - this->data[1] * other.data[0]
        };
    }
};

template<typename T>
class Vec4 : public Vector<T, 4> {
public:
    Vec4() : Vector<T, 4>() {}

    Vec4(T x, T y, T z, T w) : Vector<T, 4>({ x, y, z, w }) {}

    T& x() { return this->data[0]; }
    const T& x() const { return this->data[0]; }

    T& y() { return this->data[1]; }
    const T& y() const { return this->data[1]; }

    T& z() { return this->data[2]; }
    const T& z() const { return this->data[2]; }

    T& w() { return this->data[3]; }
    const T& w() const { return this->data[3]; }

    Vec4(const Vector<T, 4>& other) : Vector<T, 4>(other) {}
};

// Common typedefs for vector types
using Vec2f = Vec2<float>;
using Vec3f = Vec3<float>;
using Vec4f = Vec4<float>;

using Vec2i = Vec2<int>;
using Vec3i = Vec3<int>;
using Vec4i = Vec4<int>;

using Vec2ui = Vec2<unsigned int>;
using Vec3ui = Vec3<unsigned int>;
using Vec4ui = Vec4<unsigned int>;