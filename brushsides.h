#pragma once

#include "BSPElement.h"

namespace BSP {
    class BrushSide {
    public:
        // Accessor (getter) methods
        int getPlaneIndex() const { return planeIndex; }
        int getTextureID() const { return textureID; }

        // Mutator (setter) methods
        void setPlaneIndex(int newPlaneIndex) { planeIndex = newPlaneIndex; }
        void setTextureID(int newTextureID) { textureID = newTextureID; }

        // Default constructor
        BrushSide() : planeIndex(0), textureID(0) {}

        // Constructor with parameters
        BrushSide(int newPlaneIndex, int newTextureID)
            : planeIndex(newPlaneIndex), textureID(newTextureID) {}

        ~BrushSide() = default;

    private:
        int planeIndex;          // The plane index
        int textureID;           // The texture index
    };

    class BrushSides : public BSP::Element<BrushSide> {
    public:
        void validate() override;
        void displayData() const override;
    };
}
