#include "planes.h"
#include "utils.h"

namespace BSP {
    void Planes::validateData() {
        for (const auto& plane : elements) {
            float normalLength = plane.getNormal().length();

            // Check if the plane normal is a valid unit vector
            warning_assert(
                normalLength > 0.999f && normalLength < 1.001f,
                "Invalid plane normal, not a unit vector."
            );
        }
    }

    void Planes::displayData() const {
        for (const auto& plane : elements) {
            // Display plane data using getter methods
            // Add your display code here
        }
    }
}
