// pvs.cpp
#include "pvs.h"
#include "utils.h"

namespace BSP {
    void PotentiallyVisibleSet::load(std::ifstream& file, LumpData& lumpData) {
        if (lumpData.length == 0) {
            // No visibility data, set to default values
            numOfClusters = 0;
            charsPerCluster = 0;
            bitsets.clear();
            return;
        }

        file.seekg(lumpData.offset, std::ios::beg);

        file.read(reinterpret_cast<char*>(&numOfClusters), sizeof(int));
        file.read(reinterpret_cast<char*>(&charsPerCluster), sizeof(int));

        int bitsetSize = numOfClusters * charsPerCluster;
        bitsets.resize(bitsetSize);

        file.read(reinterpret_cast<char*>(bitsets.data()), bitsetSize);

        validate();
    }

    void PotentiallyVisibleSet::validate() {
        // Check if the number of clusters is positive
        warning_assert(numOfClusters > 0, "Invalid number of clusters in PVS data.");

        // Check if the number of chars per cluster is positive
        warning_assert(charsPerCluster > 0, "Invalid number of chars per cluster in PVS data.");

        // Check if the size of the bitsets vector matches the expected size
        warning_assert(static_cast<int>(bitsets.size()) == numOfClusters * charsPerCluster,
            "Invalid size of bitsets vector.");
    }

    void PotentiallyVisibleSet::displayData() const {
        // Display the number of clusters
        std::cout << "Number of clusters: " << numOfClusters << std::endl;

        // Display the number of chars per cluster
        std::cout << "Chars per cluster: " << charsPerCluster << std::endl;

        // Display the bitsets data
        std::cout << "Bitsets:" << std::endl;
        int index = 0;
        for (int i = 0; i < numOfClusters; ++i) {
            std::cout << "Cluster " << i << ": ";
            for (int j = 0; j < charsPerCluster; ++j) {
                std::cout << static_cast<int>(bitsets[index++]) << " ";
            }
            std::cout << std::endl;
        }
    }
}
