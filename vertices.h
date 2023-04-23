#pragma once

#include "BSPElement.h"
#include "vector.h"
#include "utils.h"

namespace BSP {
    // BSP vertex class
    class Vertex {
    public:
        // Accessor (getter) methods
        Vector<float, 3> getPosition() const { return position; }
        Vector<float, 2> getTextureCoord() const { return textureCoord; }
        Vector<float, 2> getLightmapCoord() const { return lightmapCoord; }
        Vector<float, 3> getNormal() const { return normal; }
        const char* getColor() const { return color; }

        // Mutator (setter) methods
        void setPosition(const Vector<float, 3>& newPosition) { position = newPosition; }
        void setTextureCoord(const Vector<float, 2>& newTextureCoord) { textureCoord = newTextureCoord; }
        void setLightmapCoord(const Vector<float, 2>& newLightmapCoord) { lightmapCoord = newLightmapCoord; }
        void setNormal(const Vector<float, 3>& newNormal) { normal = newNormal; }
        void setColor(const char newColor[4]) { std::copy(newColor, newColor + 4, color); }

        Vertex() : color{} {}
        ~Vertex() = default;

    private:
        Vector<float, 3> position;         // (x, y, z) position. 
        Vector<float, 2> textureCoord;     // (u, v) texture coordinate
        Vector<float, 2> lightmapCoord;    // (u, v) lightmap coordinate
        Vector<float, 3> normal;           // (x, y, z) normal vector
        char color[4];                     // RGBA color for the vertex
    };

    class Vertices : public BSP::Element<Vertex> {
    public:
        void load(std::ifstream& file, LumpData& lumpData) override;
        void validate() override;
        void displayData() const override;
        void updateYAndZ();
    };
}