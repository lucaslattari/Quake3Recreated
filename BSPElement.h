#ifndef BSP_ELEMENT_H_INCLUDED
#define BSP_ELEMENT_H_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "vector.h"

struct BSPLumpData
{
    int offset;		// The offset into the file for the start of this lump
    int length;		// The length in chars for this lump
};

// This is our lumps enumeration
enum LUMPS
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
    LIGHT_MAPS,			// Stores the lightmaps for the level
    LIGHT_VOLUMES,		// Stores extra world lighting information
    VISDATA,			// Stores PVS and cluster info (visibility)
    MAX_LUMPS			// A constant to store the number of lumps
};

template <typename T>
class BSPElement {
public:
    virtual void readData(std::ifstream& file, BSPLumpData& lumpData) {
        int numberOfElements = static_cast<int>(std::ceil(static_cast<float>(lumpData.length) / sizeof(T)));
        elements.resize(numberOfElements);

        file.seekg(lumpData.offset, std::ios::beg);
        file.read(reinterpret_cast<char*>(elements.data()), lumpData.length);

        validateData();
    }

    virtual void displayData() = 0;
    virtual void validateData() = 0;
protected:
    std::vector<T> elements;
};

// BSP vertex class
class BSPVertex {
public:
    Vector<float, 3> position;		// (x, y, z) position. 
    Vector<float, 2> textureCoord;	// (u, v) texture coordinate
    Vector<float, 2> lightmapCoord;	// (u, v) lightmap coordinate
    Vector<float, 3> normal;		// (x, y, z) normal vector
    char color[4];			        // RGBA color for the vertex 

    //FIX ME: Update constructors and destructors
    BSPVertex() {};
    ~BSPVertex() {};
};

class BSPVertices : public BSPElement<BSPVertex>{
public:
    void validateData() override;
    void displayData() override;
    void updateYAndZ();
};

class BSPFace {
public:
    int textureID;
    int effect;
    int type;
    int startVertIndex;
    int numOfVerts;
    int startIndex;
    int numOfIndices;
    int lightmapID;
    int lightMapCorner[2];
    int lightMapSize[2];
    Vector<float, 3> lightMapPos;
    Vector<float, 3> lightMapVecs[2];
    Vector<float, 3> normal;
    int size[2];

    //FIX ME: Update constructors and destructors
    BSPFace() {};
    ~BSPFace() {};
};

class BSPFaces : public BSPElement<BSPFace> {
public:
    void validateData() override;
    void displayData() override;
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
    void displayData() override;
};

#endif