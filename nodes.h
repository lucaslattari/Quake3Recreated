#pragma once

#include "BSPElement.h"

namespace BSP {
    class Node {
    public:
        // Default constructor
        Node() : plane(0), front(0), back(0), min({ 0,0,0 }), max({ 0,0,0 }) {}

        // Constructor with parameters
        Node(int newPlane, int newFront, int newBack, const  Vector<int, 3>& newMin, const  Vector<int, 3>& newMax)
            : plane(newPlane), front(newFront), back(newBack), min(newMin), max(newMax) {}

        // Destructor
        ~Node() = default;

        // Accessor (getter) methods
        int getPlane() const { return plane; }
        int getFront() const { return front; }
        int getBack() const { return back; }
        const Vector<int, 3>& getMin() const { return min; }
        const Vector<int, 3>& getMax() const { return max; }

        // Mutator (setter) methods
        void setPlane(int newPlane) { plane = newPlane; }
        void setFront(int newFront) { front = newFront; }
        void setBack(int newBack) { back = newBack; }
        void setMin(const Vector<int, 3>& newMin) { min = newMin; }
        void setMax(const Vector<int, 3>& newMax) { max = newMax; }

    private:
        int plane;              // The index into the planes array
        int front;              // The child index for the front node
        int back;               // The child index for the back node
        Vector<int, 3> min;     // The bounding box min position
        Vector<int, 3> max;     // The bounding box max position
    };

    class Nodes : public BSP::Element<Node> {
    public:
        void validate() override;
        void displayData() const override;
    };
}
