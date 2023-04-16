#ifndef BSP_H_INCLUDED
#define BSP_H_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "vector.h"

// This is our BSP header structure
struct BSPHeader
{
    char strID[4];	// This should always be 'IBSP'
    int version;	// This should be 0x2e for Quake 3 files
};

struct BSPLump
{
    int offset;		// The offset into the file for the start of this lump
    int length;		// The length in chars for this lump
};

// This is our BSP vertex structure
struct BSPVertex
{
    Vector<float, 3> position;		// (x, y, z) position. 
    Vector<float, 2> textureCoord;	// (u, v) texture coordinate
    Vector<float, 2> lightmapCoord;	// (u, v) lightmap coordinate
    Vector<float, 3> normal;		// (x, y, z) normal vector
    char color[4];			        // RGBA color for the vertex 
};

// BSP face structure
struct BSPFace
{
    int textureID;				        // The index into the texture array 
    int effect;					        // The index for the effects (or -1 = n/a) 
    int type;					        // 1=polygon, 2=patch, 3=mesh, 4=billboard 
    int startVertIndex;			        // The starting index into this face's first vertex 
    int numOfVerts;				        // The number of vertices for this face 
    int startIndex;				        // The starting index into the indices array for this face
    int numOfIndices;			        // The number of indices for this face
    int lightmapID;				        // The texture index for the lightmap 
    int lightMapCorner[2];	            // The face's lightmap corner in the image 
    int lightMapSize[2];		        // The size of the lightmap section 
    Vector<float, 3> lightMapPos;       // The 3D origin of lightmap. 
    Vector<float, 3> lightMapVecs[2];	// The 3D space for s and t unit vectors. 
    Vector<float, 3> normal;			// The face normal. 
    int size[2];				        // The bezier patch dimensions. 
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

class Quake3BSPLoader 
{
public:
    Quake3BSPLoader();
    ~Quake3BSPLoader();

	bool load(const std::string& filename);
private:
    BSPHeader header;
    BSPLump lumps[MAX_LUMPS];

    std::vector<BSPVertex>  vertices;
    std::vector<BSPFace>    faces;

    void displayHeaderData(BSPHeader& header);
    void displayLumpData(BSPLump(&lumps)[MAX_LUMPS]);

    void displayVertexData();
    void validateVertexData();
    void readVertexData(std::ifstream& file);

    void readFaceData(std::ifstream& file);
    void validateFaceData();
};

#endif // BSP_H_INCLUDED
