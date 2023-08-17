#pragma once

#include "vector.h"
#include <array>
#include <cassert>
#include <cmath>
#include <stdexcept>
#include <iostream>

template<typename T, int M, int N>
class Matrix {
private:
    std::array<Vector<T, N>, M> data;

public:
    Matrix() { //identity matrix
        for (int i = 0; i < M && i < N; i++) {
            data[i][i] = 1;
        }
    }

    // Construct from row vectors
    Matrix(std::initializer_list<Vector<T, N>> values) {
        if (values.size() != M) {
            throw std::length_error("Invalid number of rows");
        }
        std::copy(values.begin(), values.end(), data.begin());
    }

    // Accessors with boundary check
    T& operator()(int i, int j) {
        if (i < 0 || i >= M || j < 0 || j >= N) {
            throw std::out_of_range("Index out of range");
        }
        return data[i][j];
    }

    const T& operator()(int i, int j) const {
        if (i < 0 || i >= M || j < 0 || j >= N) {
            throw std::out_of_range("Index out of range");
        }
        return data[i][j];
    }

    const T* getData() const {
        return reinterpret_cast<const T*>(data.data());
    }

    // Identity matrix
    static Matrix<T, M, N> identity() {
        static_assert(M == N, "Identity matrix must be square");
        Matrix<T, M, N> result;
        for (int i = 0; i < M; i++) {
            result(i, i) = 1;
        }
        return result;
    }

    // Matrix multiplication
    template<int P>
    Matrix<T, M, P> operator*(const Matrix<T, N, P>& other) const {
        Matrix<T, M, P> result;
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < P; j++) {
                T sum = 0;
                for (int k = 0; k < N; k++) {
                    sum += data[i][k] * other.data[k][j];
                }
                result(i, j) = sum;
            }
        }
        return result;
    }

    // Matrix-vector multiplication
    Vector<T, M> operator*(const Vector<T, N>& vec) const {
        Vector<T, M> result;
        for (int i = 0; i < M; i++) {
            T sum = 0;
            for (int j = 0; j < N; j++) {
                sum += data[i][j] * vec[j];
            }
            result[i] = sum;
        }
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix<T, M, N>& matrix) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                os << matrix(i, j);
                if (j < N - 1) {
                    os << " ";
                }
            }
            os << std::endl;
        }
        return os;
    }
};

template<typename T>
class Mat4 : public Matrix<T, 4, 4> {
public:
    // Explicitly default the constructor to inherit the one from Matrix
    Mat4() = default;

    Mat4(std::initializer_list<T> values) {
        if (values.size() != 16) {
            throw std::length_error("Invalid number of elements");
        }
        auto it = values.begin();
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                (*this)(i, j) = *it++;
            }
        }
    }

    Mat4(const Matrix<T, 4, 4>& matrix) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                (*this)(i, j) = matrix(i, j);
            }
        }
    }

    static Mat4<T> rotate(float angle, const Vec3<T>& axis) {
        Vec3<T> u = axis.normalize();
        float s = std::sin(angle);
        float c = std::cos(angle);
        float oc = 1.0f - c;

        return Mat4<T>{
            u.x()* u.x()* oc + c, u.x()* u.y()* oc - u.z() * s, u.x()* u.z()* oc + u.y() * s, 0.0f,
                u.y()* u.x()* oc + u.z() * s, u.y()* u.y()* oc + c, u.y()* u.z()* oc - u.x() * s, 0.0f,
                u.z()* u.x()* oc - u.y() * s, u.z()* u.y()* oc + u.x() * s, u.z()* u.z()* oc + c, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
        };
    }

    Mat4<T> transpose() const {
        Mat4<T> result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result(i, j) = (*this)(j, i);
            }
        }
        return result;
    }
};

// Definição da sobrecarga do operador << fora da classe Mat4
template<typename T>
std::ostream& operator<<(std::ostream& os, const Mat4<T>& matrix) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            os << matrix(i, j);
            if (j < 3) {
                os << " ";
            }
        }
        os << std::endl;
    }
    return os;
}