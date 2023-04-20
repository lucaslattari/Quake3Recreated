#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "BSPElement.h"
#include "BSPVertices.h"
#include "BSPFaces.h"
#include "vector.h"

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
    BSPLumpData lumps[static_cast<int>(LUMPS::MAXLUMPS)];

    BSPVertices vertices;
    BSPFaces    faces;
    BSPTextures textures;
    BSPLightmaps lightmaps;

    void displayHeaderData(BSPHeader& header);
    void displayLumpData(BSPLumpData(&lumps)[static_cast<int>(LUMPS::MAXLUMPS)]);
};
