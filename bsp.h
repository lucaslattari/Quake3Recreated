#pragma once

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "GL_Utils.h"

#include "BSPelement.h"
#include "vertices.h"
#include "faces.h"
#include "textures.h"
#include "lightmaps.h"
#include "nodes.h"
#include "leaves.h"
#include "planes.h"
#include "pvs.h"
#include "brushes.h"
#include "brushsides.h"

#include "vector.h"
#include "indexedData.h"
#include "bezierPatches.h"

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
        void loadLumps(std::ifstream& file);        
        void drawLevel(const Vec3<float>& vPos);

    private:
        Header header;
        LumpData lumps[static_cast<int>(LUMPS::MAXLUMPS)];

        Vertices                vertices;
        Faces                   faces;
        Textures                textures;
        Lightmaps               lightmaps;
        Nodes                   nodes;
        Leaves                  leaves;
        Planes                  planes;
        PotentiallyVisibleSet   pvs;
        Brushes                 brushes;
        BrushSides              brushsides;
        IndexedData             indices;
        IndexedData             leafFaces;
        IndexedData             leafBrushes;

        GLuint globalVAO, globalVBO;
        std::unordered_map<int, int> faceToOffsetMap;
        std::vector<PatchData> patches;

        void displayHeaderData(Header& header);
        void displayLumpData(LumpData(&lumps)[static_cast<int>(LUMPS::MAXLUMPS)]);
        void drawFace(int faceIndex);

        void initializeBezierPatches();
        void initializeFaces();
        void initializeQuadraticPatches(PatchData& patchData, const Face& face);
    };
}