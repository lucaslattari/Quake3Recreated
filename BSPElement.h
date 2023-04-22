#pragma once

#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <vector>

#include "vector.h"

namespace BSP {
    struct LumpData
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
        LEAVES,				// Stores the leaves of the nodes
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
    class Element {
    public:
        Element() = default;
        virtual ~Element() = default;

        virtual void readData(std::ifstream& file, LumpData& lumpData) {
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
}