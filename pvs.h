#pragma once

#include "BSPElement.h"
#include <vector>

/*
PVS stores the visibility information of clusters in the Quake 3 BSP (Binary Space Partitioning) 
format. 

It is used to represent and store the Potentially Visible Set (PVS) data for each cluster in the 
BSP tree. The PVS data helps to optimize rendering by determining which areas of the map are 
potentially visible from a particular cluster or area.

The class has the following members:

numOfClusters: An integer representing the total number of clusters in the BSP tree. A cluster 
is a group of leaves (smallest convex regions) in the BSP tree that share visibility information.

charsPerCluster: An integer representing the number of characters (each character is 8 bits) 
required to store the bitset for a single cluster's visibility data. The bitset indicates which 
other clusters are visible from the current cluster.

pBitsets: A pointer to an array of characters that hold the actual cluster bitsets. This is a 
one-dimensional array storing the packed bitset data for all clusters, with each cluster's data 
occupying charsPerCluster characters.
*/

namespace BSP {
    class PotentiallyVisibleSet {
    public:
        // Default constructor
        PotentiallyVisibleSet() : numOfClusters(0), charsPerCluster(0), bitsets() {}

        // Constructor with parameters
        PotentiallyVisibleSet(int numOfClusters, int charsPerCluster, const std::vector<char>& bitsets)
            : numOfClusters(numOfClusters), charsPerCluster(charsPerCluster), bitsets(bitsets) {}

        // Accessor (getter) methods
        int getNumOfClusters() const { return numOfClusters; }
        int getCharsPerCluster() const { return charsPerCluster; }
        const std::vector<char>& getBitsets() const { return bitsets; }

        // Read PVS data from file
        void load(std::ifstream& file, LumpData& lumpData);

        // Validate PVS data
        void validate();

        // Display PVS data
        void displayData() const;

    private:
        int numOfClusters;                     // The number of clusters
        int charsPerCluster;                   // The amount of chars (8 bits) in the cluster's bitset
        std::vector<char> bitsets;             // The vector of chars that holds the cluster bitsets
    };
}
