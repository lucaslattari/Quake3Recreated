#pragma once

#include <array>
#include <cmath>
#include <initializer_list>
#include <algorithm>
#include <cassert>

template <typename T, int N>
class VectorBase {
protected:
    union {
        std::array<T, N> data;
        struct {
            T x, y, z, w;
        };
    };

public:
    T& operator[](int index) {
        return data[index];
    }

    const T& operator[](int index) const {
        return data[index];
    }

    VectorBase() : data{} {}

    VectorBase(const T* values) {
        std::copy_n(values, N, data.begin());
    }

    VectorBase(std::initializer_list<T> values) {
        assert(values.size() <= N);
        std::copy(values.begin(), values.end(), data.begin());
    }

    // Arithmetic operations
    VectorBase operator+(const VectorBase& other) const {
        VectorBase result;
        for (int i = 0; i < N; i++) {
            result.data[i] = data[i] + other.data[i];
        }
        return result;
    }

    VectorBase& operator+=(const VectorBase& other) {
        for (int i = 0; i < N; i++) {
            data[i] += other.data[i];
        }
        return *this;
    }

    VectorBase operator-(const VectorBase& other) const {
        VectorBase result;
        for (int i = 0; i < N; i++) {
            result.data[i] = data[i] - other.data[i];
        }
        return result;
    }

    VectorBase operator*(T scalar) const {
        VectorBase result;
        for (int i = 0; i < N; i++) {
            result.data[i] = data[i] * scalar;
        }
        return result;
    }

    // Dot product
    T dot(const VectorBase& other) const {
        T result = 0;
        for (int i = 0; i < N; i++) {
            result += data[i] * other.data[i];
        }
        return result;
    }

    // Length and normalization
    T length() const {
        T sum = 0;
        for (int i = 0; i < N; i++) {
            sum += data[i] * data[i];
        }
        return sqrt(sum);
    }

    void normalize() {
        T len = length();
        for (int i = 0; i < N; i++) {
            data[i] /= len;
        }
    }

    VectorBase normalized() const {
        VectorBase result = static_cast<const VectorBase&>(*this);
        result.normalize();
        return result;
    }
};

template<typename T, int N, typename Derived>
class Vector : public VectorBase<T, N> {
public:
    using VectorBase<T, N>::VectorBase;

    // Cast to derived class
    operator const Derived& () const {
        return static_cast<const Derived&>(*this);
    }

    operator Derived& () {
        return static_cast<Derived&>(*this);
    }

    // Arithmetic operations
    Derived operator+(const Derived& other) const {
        Derived result;
        for (int i = 0; i < N; i++) {
            result[i] = (*this)[i] + other[i];
        }
        return result;
    }

    Derived operator-(const Derived& other) const {
        Derived result;
        for (int i = 0; i < N; i++) {
            result[i] = (*this)[i] - other[i];
        }
        return result;
    }

    Derived operator*(T scalar) const {
        Derived result;
        for (int i = 0; i < N; i++) {
            result[i] = (*this)[i] * scalar;
        }
        return result;
    }

    // Dot product
    T dot(const Derived& other) const {
        T result = 0;
        for (int i = 0; i < N; i++) {
            result += (*this)[i] * other[i];
        }
        return result;
    }

    // Length and normalization
    T length() const {
        T sum = 0;
        for (int i = 0; i < N; i++) {
            sum += (*this)[i] * (*this)[i];
        }
        return sqrt(sum);
    }

    void normalize() {
        T len = length();
        for (int i = 0; i < N; i++) {
            (*this)[i] /= len;
        }
    }

    Derived normalized() const {
        Derived result = static_cast<const Derived&>(*this);
        result.normalize();
        return result;
    }
};

// Specializations for common 2D vector types
template<typename T>
class Vec2 : public Vector<T, 2, Vec2<T>> {
public:
    Vec2() : Vector<T, 2, Vec2<T>>() {}

    Vec2(T x, T y) : Vector<T, 2, Vec2<T>>({ x, y }) {}

    operator Vector<T, 2, Vec2<T>>() const {
        return *this;
    }
};

template<typename T>
class Vec3 : public Vector<T, 3, Vec3<T>> {
public:
    Vec3() : Vector<T, 3, Vec3<T>>() {}

    Vec3(T x, T y, T z) : Vector<T, 3, Vec3<T>>({ x, y, z }) {}

    Vec3 cross(const Vec3& other) const {
        Vec3 result;
        result[0] = (*this)[1] * other[2] - (*this)[2] * other[1];
        result[1] = (*this)[2] * other[0] - (*this)[0] * other[2];
        result[2] = (*this)[0] * other[1] - (*this)[1] * other[0];
        return result;
    }

    operator Vector<T, 3, Vec3<T>>() const {
        return *this;
    }
};

template<typename T>
class Vec4 : public Vector<T, 4, Vec4<T>> {
public:
    Vec4() : Vector<T, 4, Vec4<T>>() {}

    Vec4(T x, T y, T z, T w) {
        this->data[0] = x;
        this->data[1] = y;
        this->data[2] = z;
        this->data[3] = w;
    }

    operator Vector<T, 4, Vec4<T>>() const {
        return *this;
    }
};

template<typename T, int N>
class SimpleVector : public Vector<T, N, SimpleVector<T, N>> {
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