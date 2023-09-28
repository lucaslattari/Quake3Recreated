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
class Mat3 : public Matrix<T, 3, 3> {
public:
    // Explicitly default the constructor to inherit the one from Matrix
    Mat3() = default;

    Mat3(std::initializer_list<T> values) {
        if (values.size() != 9) {
            throw std::length_error("Invalid number of elements");
        }
        auto it = values.begin();
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                (*this)(i, j) = *it++;
            }
        }
    }

    Mat3(const Matrix<T, 3, 3>& matrix) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                (*this)(i, j) = matrix(i, j);
            }
        }
    }

    Vec3<T> operator*(const Vec3<T>& vec) const {
        Vec3<T> result;
        for (int i = 0; i < 3; i++) {
            T sum = 0;
            for (int j = 0; j < 3; j++) {
                sum += (*this)(i, j) * vec[j];
            }
            result[i] = sum;
        }
        return result;
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

    static Mat4<T> rotate(T angle, const Vec3<T>& axis) {
        Vec3<T> u = axis.normalize();
        float s = std::sin(static_cast<T>(angle));
        float c = std::cos(static_cast<T>(angle));
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

    Mat4<T> inverse() const {
        Mat4<T> inv;
        T det;

        //T* m = reinterpret_cast<T*>(this->getData());
        //T* invOut = reinterpret_cast<T*>(inv.getData());

        std::array<T, 16> m_copy;
        std::copy(this->getData(), this->getData() + 16, m_copy.begin());
        T* m = m_copy.data();

        std::copy(inv.getData(), inv.getData() + 16, m_copy.begin());
        T* invOut = m_copy.data();

        // First row (Cramer method)
        invOut[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
        invOut[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
        invOut[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
        invOut[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];

        // Calculate determinant
        det = m[0] * invOut[0] + m[1] * invOut[4] + m[2] * invOut[8] + m[3] * invOut[12];
        if (det == 0)
            throw std::runtime_error("Matrix is singular and cannot be inverted.");

        det = 1.0 / det;

        // Second row
        invOut[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
        invOut[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
        invOut[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
        invOut[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];

        // Third row
        invOut[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
        invOut[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
        invOut[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
        invOut[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];

        // Fourth row
        invOut[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
        invOut[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
        invOut[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
        invOut[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

        // Multiply each element by 1/det
        for (int i = 0; i < 16; i++) {
            invOut[i] *= det;
        }

        return inv;
    }

    Mat3<T> getSubmatrix3x3(int rowToRemove, int colToRemove) const {
        Mat3<T> submatrix;
        int subRow = 0, subCol = 0;

        for (int row = 0; row < 4; ++row) {
            if (row == rowToRemove) continue;

            subCol = 0;
            for (int col = 0; col < 4; ++col) {
                if (col == colToRemove) continue;

                submatrix(subRow, subCol) = (*this)(row, col);
                ++subCol;
            }

            ++subRow;
        }

        return submatrix;
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