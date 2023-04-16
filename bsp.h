#ifndef BSP_H_INCLUDED
#define BSP_H_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "vector.h"
#include "BSPElement.h"

// This is our BSP header structure
struct BSPHeader
{
    char strID[4];	// This should always be 'IBSP'
    int version;	// This should be 0x2e for Quake 3 files
};

class Quake3BSPLoader 
{
public:
    Quake3BSPLoader();
    ~Quake3BSPLoader();

	bool load(const std::string& filename);
private:
    BSPHeader header;
    BSPLumpData lumps[MAX_LUMPS];

    BSPVertices vertices;
    BSPFaces    faces;
    BSPTextures textures;

    void displayHeaderData(BSPHeader& header);
    void displayLumpData(BSPLumpData(&lumps)[MAX_LUMPS]);
};

#endif // BSP_H_INCLUDED
