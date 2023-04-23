#pragma once

#include "BSPElement.h"
#include "vector.h"

namespace BSP {
    class Leaf {
    public:
        // Accessor (getter) methods
        int getCluster() const { return cluster; }
        int getArea() const { return area; }
        const Vector<int, 3>& getMin() const { return min; }
        const Vector<int, 3>& getMax() const { return max; }
        int getLeafFace() const { return leafface; }
        int getNumOfLeafFaces() const { return numOfLeafFaces; }
        int getLeafBrush() const { return leafBrush; }
        int getNumOfLeafBrushes() const { return numOfLeafBrushes; }

        // Mutator (setter) methods
        void setCluster(int newCluster) { cluster = newCluster; }
        void setArea(int newArea) { area = newArea; }
        void setMin(const Vector<int, 3>& newMin) { min = newMin; }
        void setMax(const Vector<int, 3>& newMax) { max = newMax; }
        void setLeafFace(int newLeafFace) { leafface = newLeafFace; }
        void setNumOfLeafFaces(int newNumOfLeafFaces) { numOfLeafFaces = newNumOfLeafFaces; }
        void setLeafBrush(int newLeafBrush) { leafBrush = newLeafBrush; }
        void setNumOfLeafBrushes(int newNumOfLeafBrushes) { numOfLeafBrushes = newNumOfLeafBrushes; }

        // Default constructor
        Leaf() : cluster(0), area(0), min({ 0,0,0 }), max({ 0,0,0 }), leafface(0), numOfLeafFaces(0), leafBrush(0), numOfLeafBrushes(0) {}

        // Constructor with parameters
        Leaf(int newCluster, int newArea, const Vector<int, 3>& newMin, const Vector<int, 3>& newMax, int newLeafface, int newNumOfLeafFaces, int newLeafBrush, int newNumOfLeafBrushes)
            : cluster(newCluster), area(newArea), min(newMin), max(newMax), leafface(newLeafface), numOfLeafFaces(newNumOfLeafFaces), leafBrush(newLeafBrush), numOfLeafBrushes(newNumOfLeafBrushes) {}

        ~Leaf() = default;

    private:
        int cluster;             // The visibility cluster
        int area;                // The area portal
        Vector<int, 3> min;      // The bounding box min position
        Vector<int, 3> max;      // The bounding box max position
        int leafface;            // The first index into the face array
        int numOfLeafFaces;      // The number of faces for this leaf
        int leafBrush;           // The first index for into the brushes
        int numOfLeafBrushes;    // The number of brushes for this leaf
    };

    class Leaves : public BSP::Element<Leaf> {
    public:
        void load(std::ifstream& file, LumpData& lumpData) override;
        void updateYAndZ();
        void validate() override;
        void displayData() const override;
    };
}
