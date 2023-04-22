#pragma once

#include "BSPElement.h"

namespace BSP {
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
        Vector<float, 3> getLightMapPos() const { return lightMapPos; }
        const Vector<float, 3>* getLightMapVecs() const { return lightMapVecs; }
        Vector<float, 3> getNormal() const { return normal; }
        const int* getSize() const { return size; }

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
            size{ 0,0 } {}

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
        Vector<float, 3> lightMapPos;
        Vector<float, 3> lightMapVecs[2];
        Vector<float, 3> normal;
        int size[2];
    };

    class Faces : public BSP::Element<Face> {
    public:
        void validateData() override;
        void displayData() const override;
    };
}