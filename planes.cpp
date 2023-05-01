#include "planes.h"
#include "utils.h"

namespace BSP {
    void Planes::load(std::ifstream& file, LumpData& lumpData) {
        Element::load(file, lumpData);
        updateYAndZ();
    }

    // Swap the y and z values, and negate the new z so Y is up 
    // the swapping of y and z values and negating the new z value is related 
    // to the differences in coordinate systems used in the Quake 3 engine and 
    // most 3D modeling software
    void Planes::updateYAndZ() {
        for (auto& plane : elements) {
            Vec3<float> normal = plane.getNormal();

            normal.y = plane.getNormal().z;
            normal.z = -plane.getNormal().y;

            plane.setNormal(normal);
        }
    }

    void Planes::validate() {
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
