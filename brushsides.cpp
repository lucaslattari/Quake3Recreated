#include "brushsides.h"
#include "utils.h"

namespace BSP {
    void BrushSides::validate() {
        for (const auto& brushSide : elements) {
            // Validate brush side data here
            // For example, you can check if the planeIndex or textureID are valid
        }
    }

    void BrushSides::displayData() const {
        for (const auto& brushSide : elements) {
            // Display brush side data using getter methods
            // Add your display code here
            // Example:
            // std::cout << "Plane index: " << brushSide.getPlaneIndex()
            //           << ", Texture ID: " << brushSide.getTextureID() << std::endl;
        }
    }
}
