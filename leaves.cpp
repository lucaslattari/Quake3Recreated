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
            int temp = leaf.getMin().y();
            leaf.setMin({ leaf.getMin().x(), leaf.getMin().z(), -temp});

            // Swap the max y and z values, then negate the new Z
            temp = leaf.getMax().y();
            leaf.setMax({ leaf.getMax().x(), leaf.getMax().z(), -temp});
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
        std::cout << "Displaying Leaf Data:" << std::endl;
        int count = 1;
        for (const auto& leaf : elements) {
            std::cout << "Leaf " << count << ": " << std::endl;
            std::cout << "  Cluster: " << leaf.getCluster() << std::endl;
            std::cout << "  Area: " << leaf.getArea() << std::endl;

            const auto& min = leaf.getMin();
            std::cout << "  Min: [" << min[0] << ", " << min[1] << ", " << min[2] << "]" << std::endl;

            const auto& max = leaf.getMax();
            std::cout << "  Max: [" << max[0] << ", " << max[1] << ", " << max[2] << "]" << std::endl;

            std::cout << "  LeafFace: " << leaf.getLeafFace() << std::endl;
            std::cout << "  NumOfLeafFaces: " << leaf.getNumOfLeafFaces() << std::endl;
            std::cout << "  LeafBrush: " << leaf.getLeafBrush() << std::endl;
            std::cout << "  NumOfLeafBrushes: " << leaf.getNumOfLeafBrushes() << std::endl;

            count++;
        }
    }
}