#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "BSPelement.h"
#include "vertices.h"
#include "faces.h"
#include "textures.h"
#include "lightmaps.h"
#include "nodes.h"
#include "leaves.h"
#include "planes.h"

#include "vector.h"

namespace BSP {
    // This is our BSP header structure
    struct Header
    {
        char strID[4];	// This should always be 'IBSP'
        int version;	// This should be 0x2e for Quake 3 files
    };

    class Loader
    {
    public:
        Loader();
        ~Loader();

        bool load(const std::string& filename);
    private:
        Header header;
        LumpData lumps[static_cast<int>(LUMPS::MAXLUMPS)];

        Vertices    vertices;
        Faces       faces;
        Textures    textures;
        Lightmaps   lightmaps;
        Nodes       nodes;
        Leaves      leaves;
        Planes      planes;

        void displayHeaderData(Header& header);
        void displayLumpData(LumpData(&lumps)[static_cast<int>(LUMPS::MAXLUMPS)]);
    };
}