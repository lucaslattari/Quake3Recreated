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
        std::cout << "Displaying Node Data:" << std::endl;
        int count = 1;
        for (const auto& node : elements) {
            std::cout << "Node " << count << ": " << std::endl;
            std::cout << "  Plane: " << node.getPlane() << std::endl;
            std::cout << "  Front: " << node.getFront() << std::endl;
            std::cout << "  Back: " << node.getBack() << std::endl;

            const auto& min = node.getMin();
            std::cout << "  Min: [" << min[0] << ", " << min[1] << ", " << min[2] << "]" << std::endl;

            const auto& max = node.getMax();
            std::cout << "  Max: [" << max[0] << ", " << max[1] << ", " << max[2] << "]" << std::endl;

            count++;
        }
    }
}