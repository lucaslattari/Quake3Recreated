#include "leaves.h"
#include "utils.h"

namespace BSP {
    void Leaves::validateData() {
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