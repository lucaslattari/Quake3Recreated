#include "leaves.h"
#include "utils.h"

namespace BSP {
    void Leaves::load(std::ifstream& file, LumpData& lumpData) {
        Element::load(file, lumpData);

        updateYAndZ();
    }

    void Leaves::updateYAndZ() {
        for (auto& leaf : elements) {
            // Swap the min y and z values, then negate the new Z
            int temp = leaf.getMin().y;
            leaf.setMin({ leaf.getMin().x, leaf.getMin().z, -temp });

            // Swap the max y and z values, then negate the new Z
            temp = leaf.getMax().y;
            leaf.setMax({ leaf.getMax().x, leaf.getMax().z, -temp });
        }
    }

    void Leaves::validate() {
        for (const auto& leaf : elements) {
            warning_assert(leaf.getCluster() >= 0, "Cluster is negative.");
            warning_assert(leaf.getArea() >= -1, "Area is less than -1.");
            warning_assert(leaf.getNumOfLeafFaces() >= 0, "numOfLeafFaces is negative.");
            warning_assert(leaf.getNumOfLeafBrushes() >= 0, "numOfLeafBrushes is negative.");
            warning_assert(leaf.getMin()[0] <= leaf.getMax()[0] &&
                leaf.getMin()[1] <= leaf.getMax()[1] &&
                leaf.getMin()[2] <= leaf.getMax()[2], "Invalid bounding box.");
        }
    }

    void Leaves::displayData() const {
        for (const auto& leaf : elements) {
            // Display leaf data using getter methods
            // Add your display code here
        }
    }
}