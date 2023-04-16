#include <iostream>

#include "utils.h"

void warning_assert(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "Assertion failed: " << message << std::endl;
    }
}