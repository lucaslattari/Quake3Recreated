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
        std::cout << "Displaying Brush Data:" << std::endl;
        int count = 1;
        for (const auto& brush : elements) {
            std::cout << "Brush " << count << ": " << std::endl;
            std::cout << "  Brush Side: " << brush.getBrushSide() << std::endl;
            std::cout << "  Number of Brush Sides: " << brush.getNumOfBrushSides() << std::endl;
            std::cout << "  Texture ID: " << brush.getTextureID() << std::endl;
            count++;
        }
    }
}
