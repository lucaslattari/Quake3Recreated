#include <array>
#include <cmath>
#include <initializer_list>
#include <algorithm>
#include <cassert>

template<typename T, int N>
class Vector {
public:
    union {
        std::array<T, N> data;
        struct {
            T x, y, z, w;
        };
    };

    Vector() : data{} {}

    Vector(const T* values) {
        std::copy_n(values, N, data.begin());
    }

    Vector(std::initializer_list<T> values) {
        assert(values.size() <= N);
        std::copy(values.begin(), values.end(), data.begin());
    }

    // Arithmetic operations
    Vector operator+(const Vector& other) const {
        Vector result;
        for (int i = 0; i < N; i++) {
            result.data[i] = data[i] + other.data[i];
        }
        return result;
    }

    Vector operator-(const Vector& other) const {
        Vector result;
        for (int i = 0; i < N; i++) {
            result.data[i] = data[i] - other.data[i];
        }
        return result;
    }

    Vector operator*(T scalar) const {
        Vector result;
        for (int i = 0; i < N; i++) {
            result.data[i] = data[i] * scalar;
        }
        return result;
    }

    // Dot product
    T dot(const Vector& other) const {
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

    Vector<T, N> normalized() const {
        Vector<T, N> result = *this;
        result.normalize();
        return result;
    }
};

// Specializations for common 2D vector types
template<typename T>
class Vec2 : public Vector<T, 2> {
public:
    Vec2() : Vector<T, 2>() {}
};

template<typename T>
class Vec3 : public Vector<T, 3> {
public:
    Vec3() : Vector<T, 3>() {}

    Vector<T, 3> cross(const Vector<T, 3>& other) const {
        Vector<T, 3> result;
        result.data[0] = this->data[1] * other.data[2] - this->data[2] * other.data[1];
        result.data[1] = this->data[2] * other.data[0] - this->data[0] * other.data[2];
        result.data[2] = this->data[0] * other.data[1] - this->data[1] * other.data[0];
        return result;
    }
};

template<typename T>
class Vec4 : public Vector<T, 4> {
public:
    Vec4() : Vector<T, 4>() {}
};
