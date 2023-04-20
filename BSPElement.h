#pragma once

#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <vector>

#include "vector.h"

struct BSPLumpData
{
    int offset;		// The offset into the file for the start of this lump
    int length;		// The length in chars for this lump
};

enum class LUMPS
{
    ENTITIES = 0,	    // Stores player/object positions, etc...
    TEXTURES,			// Stores texture information
    PLANES,				// Stores the splitting planes
    NODES,				// Stores the BSP nodes
    LEAFS,				// Stores the leafs of the nodes
    LEAF_FACES,			// Stores the leaf's indices into the faces
    LEAF_BRUSHES,		// Stores the leaf's indices into the brushes
    MODELS,				// Stores the info of world models
    BRUSHES,			// Stores the brushes info (for collision)
    BRUSH_SIDES,		// Stores the brush surfaces info
    VERTICES,			// Stores the level vertices
    INDICES,			// Stores the level indices
    SHADERS,			// Stores the shader files (blending, anims..)
    FACES,				// Stores the faces for the level
    LIGHTMAPS,			// Stores the lightmaps for the level
    LIGHTVOLUMES,		// Stores extra world lighting information
    VISDATA,			// Stores PVS and cluster info (visibility)
    MAXLUMPS			// A constant to store the number of lumps
};

template <typename T>
class BSPElement {
public:
    BSPElement() = default;
    virtual ~BSPElement() = default;

    virtual void readData(std::ifstream& file, BSPLumpData& lumpData) {
        int numberOfElements = static_cast<int>(std::ceil(static_cast<float>(lumpData.length) / sizeof(T)));
        elements.resize(numberOfElements);

        file.seekg(lumpData.offset, std::ios::beg);
        file.read(reinterpret_cast<char*>(elements.data()), lumpData.length);

        validateData();
    }

    virtual void displayData() const = 0;
    virtual void validateData() = 0;
protected:
    std::vector<T> elements;
};

// BSP texture class
class BSPTexture {
public:
    char name[64];
    int flags;
    int textureType;

    BSPTexture() {};
    ~BSPTexture() {};
};

// BSP texture class
class BSPTextures : public BSPElement<BSPTexture> {
public:
    void validateData() override;
    void displayData() const override;
};

class BSPLightmap {
public:
    BSPLightmap() {};
    ~BSPLightmap() {};

    // The RGB data in a 128x128 image
    std::array<std::array<std::array<char, 3>, 128>, 128> imageBits; 
};

class BSPLightmaps : public BSPElement<BSPLightmap> {
public:
    void validateData() override;
    void displayData() const override;
};

class BSPNode {
    int plane;                  // The index into the planes array
    int front;                  // The child index for the front node
    int back;                   // The child index for the back node
    std::array<int, 3> min;     // The bounding box min position
    std::array<int, 3> max;     // The bounding box max position
};