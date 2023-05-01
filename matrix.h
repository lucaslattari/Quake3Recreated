#pragma once

#include "vector.h"
#include <cassert>

template<typename T, int M, int N>
class Matrix {
private:
    std::array<Vector<T, N, SimpleVector<T, N>>, M> data;

public:
    Matrix() : data{} {}

    // Construct from row vectors
    Matrix(std::initializer_list<Vector<T, N, SimpleVector<T, N>>> values) {
        assert(values.size() == M);
        int i = 0;
        for (const auto& row : values) {
            data[i] = row;
            i++;
        }
    }

    // Accessors
    T& operator()(int i, int j) {
        return data[i][j];
    }

    const T& operator()(int i, int j) const {
        return data[i][j];
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
    Vector<T, M, SimpleVector<T, M>> operator*(const Vector<T, N, SimpleVector<T, N>>& vec) const {
        Vector<T, M, SimpleVector<T, M>> result;
        for (int i = 0; i < M; i++) {
            T sum = 0;
            for (int j = 0; j < N; j++) {
                sum += data[i][j] * vec[j];
            }
            result[i] = sum;
        }
        return result;
    }
};