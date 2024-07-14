#pragma once

#include <cmath>

// Definir M_PI se n�o estiver definido
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Fun��o para converter graus para radianos
inline float radians(float degrees) {
    return degrees * static_cast<float>(M_PI) / 180.0f;
}
