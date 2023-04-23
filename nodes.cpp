#include "Nodes.h"
#include "utils.h"

namespace BSP {
    void Nodes::validate() {
        for (const auto& node : elements) {
            warning_assert(node.getPlane() >= 0, "Plane is negative.");

            warning_assert(node.getFront() >= -1, "Front is less than -1.");

            warning_assert(node.getBack() >= -1, "Back is less than -1.");

            warning_assert(node.getMin()[0] <= node.getMax()[0] &&
                node.getMin()[1] <= node.getMax()[1] &&
                node.getMin()[2] <= node.getMax()[2], "Invalid bounding box.");

        }
    }

    void Nodes::displayData() const {
        for (const auto& node : elements) {
            // Display node data using getter methods
            // Add your display code here
        }
    }
}