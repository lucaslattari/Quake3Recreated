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
        std::cout << "Displaying BrushSide Data:" << std::endl;
        int count = 1;
        for (const auto& brushSide : elements) {
            std::cout << "BrushSide " << count << ": " << std::endl;
            std::cout << "  Plane Index: " << brushSide.getPlaneIndex() << std::endl;
            std::cout << "  Texture ID: " << brushSide.getTextureID() << std::endl;
            count++;
        }
    }
}
