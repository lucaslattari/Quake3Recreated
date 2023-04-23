#pragma once

#include "BSPElement.h"
#include "vector.h"

namespace BSP {
    class Plane {
    public:
        // Accessor (getter) methods
        const Vector<float, 3>& getNormal() const { return normal; }
        float getDistanceFromOrigin() const { return distanceFromOrigin; }

        // Mutator (setter) methods
        void setNormal(const Vector<float, 3>& newNormal) { normal = newNormal; }
        void setDistanceFromOrigin(float newDistanceFromOrigin) { distanceFromOrigin = newDistanceFromOrigin; }

        // Default constructor
        Plane() : normal({ 0.0f, 0.0f, 0.0f }), distanceFromOrigin(0.0f) {}

        // Constructor with parameters
        Plane(const Vector<float, 3>& newNormal, float newDistanceFromOrigin)
            : normal(newNormal), distanceFromOrigin(newDistanceFromOrigin) {}

        ~Plane() = default;

    private:
        Vector<float, 3> normal;          // Plane normal.
        float distanceFromOrigin;         // The plane distance from origin
    };

    class Planes : public BSP::Element<Plane> {
    public:
        void load(std::ifstream& file, LumpData& lumpData) override;
        void updateYAndZ();
        void validate() override;
        void displayData() const override;
    };
}
