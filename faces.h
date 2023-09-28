#pragma once

#include "BSPElement.h"

namespace BSP {
    enum FaceType {
        FACE_POLYGON = 1,
        FACE_PATCH = 2,
        FACE_MESH = 3,
        FACE_BBOARD = 4
    };

    class Face {
    public:
        // Accessor (getter) methods
        int getTextureID() const { return textureID; }
        int getEffect() const { return effect; }
        int getType() const { return type; }
        int getStartVertIndex() const { return startVertIndex; }
        int getNumOfVerts() const { return numOfVerts; }
        int getStartIndex() const { return startIndex; }
        int getNumOfIndices() const { return numOfIndices; }
        int getLightmapID() const { return lightmapID; }
        const int* getLightMapCorner() const { return lightMapCorner; }
        const int* getLightMapSize() const { return lightMapSize; }
        Vec3<float> getLightMapPos() const { return lightMapPos; }
        const Vec3<float>* getLightMapVecs() const { return lightMapVecs; }
        Vec3<float> getNormal() const { return normal; }
        const int* getBezierPatchesSize() const { return bezierPatchesSize; }

        friend std::ostream& operator<<(std::ostream& os, const Face& face);

        // Mutator (setter) methods
        // Add setter methods if needed

        Face() : 
            textureID(0), 
            effect(0),
            type(0), 
            startVertIndex(0), 
            numOfVerts(0), 
            startIndex(0), 
            numOfIndices(0), 
            lightmapID(0), 
            lightMapCorner{ 0,0 }, 
            lightMapSize{ 0,0 }, 
            bezierPatchesSize{ 0,0 } {}

        ~Face() = default;

    private:
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
        Vec3f lightMapPos;
        Vec3f lightMapVecs[2];
        Vec3f normal;
        int bezierPatchesSize[2];
    };

    class Faces : public BSP::Element<Face> {
    public:
        void validate() override;
        void displayData() const override;
    };
}