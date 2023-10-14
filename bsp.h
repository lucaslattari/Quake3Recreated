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
        void drawLevel(const Vec3<float>& vPos, GLuint shaderProgram);

        void setRenderPolygonsAndMeshes(bool value) { renderPolygonsAndMeshes = value; }
        void setRenderPatches(bool value) { renderPatches = value; }

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

        GLuint faceVAO, faceVBO;
        GLuint patchVAO, patchVBO, patchEBO;
        GLuint shaderProgram;

        int tesselationLevel;

        bool renderPolygonsAndMeshes = true;  // Flag para renderizar Polygons e Meshes
        bool renderPatches = true;            // Flag para renderizar Patches

        std::unordered_map<int, int> faceToOffsetMap;
        std::vector<PatchData> patches;
        std::unordered_map<int, int> patchToOffsetMap;
        std::unordered_map<int, GLuint> indexPatchToOffsetMap;

        void displayHeaderData(Header& header);
        void displayLumpData(LumpData(&lumps)[static_cast<int>(LUMPS::MAXLUMPS)]);
        void drawFace(int faceIndex);

        void initializeBezierPatches();
        void initializeFaces();
        void initializeQuadraticPatches(PatchData& patchData, const Face& face, int tesselationLevel);

        void setTesselationLevel(int level);
        void debugVBO(GLuint vbo, GLsizei size);
        void debugEBO(GLuint ebo, size_t size);
    };
}