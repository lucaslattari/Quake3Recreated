#pragma once

#include "BSPElement.h"
#include "vector.h"
#include "utils.h"

#include <iostream>

namespace BSP {
    // BSP vertex class
    class Vertex {
    public:
        // Accessor (getter) methods
        Vec3f getPosition() const { return position; }
        Vec2f getTextureCoord() const { return textureCoord; }
        Vec2f getLightmapCoord() const { return lightmapCoord; }
        Vec3f getNormal() const { return normal; }
        const char* getColor() const { return color; }

        // Mutator (setter) methods
        void setPosition(const Vec3f& newPosition) { position = newPosition; }
        void setTextureCoord(const Vec2f& newTextureCoord) { textureCoord = newTextureCoord; }
        void setLightmapCoord(const Vec2f& newLightmapCoord) { lightmapCoord = newLightmapCoord; }
        void setNormal(const Vec3f& newNormal) { normal = newNormal; }

        //TODO: tirar esse char[4] daqui
        void setColor(const Vec4f& newColor) {
            for (int i = 0; i < 4; ++i) {
                color[i] = static_cast<char>(newColor[i]);
            }
        }

        Vertex() : color{} {}
        ~Vertex() = default;

    private:
        Vec3f position;         // (x, y, z) position. 
        Vec2f textureCoord;     // (u, v) texture coordinate
        Vec2f lightmapCoord;    // (u, v) lightmap coordinate
        Vec3f normal;           // (x, y, z) normal vector
        char color[4];          // RGBA color for the vertex
    };

    std::ostream& operator<<(std::ostream& os, const Vertex& vertex);

    class Vertices : public BSP::Element<Vertex> {
    public:
        void load(std::ifstream& file, LumpData& lumpData) override;
        void validate() override;
        void displayData() const override;
        void updateYAndZ();
    };
}