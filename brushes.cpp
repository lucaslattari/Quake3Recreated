#include "brushes.h"
#include "utils.h"

namespace BSP {
    void Brushes::validate() {
        for (const auto& brush : elements) {
            warning_assert(brush.getBrushSide() >= 0, "Brush side is negative.");
            warning_assert(brush.getNumOfBrushSides() >= 0, "Number of brush sides is negative.");
            warning_assert(brush.getTextureID() >= 0, "Texture ID is negative.");
        }
    }

    void Brushes::displayData() const {
        for (const auto& brush : elements) {
            // Display brush data using getter methods
            // Add your display code here
        }
    }
}
