#pragma once

#include "BSPElement.h"

/*
In the context of the Quake 3 engine, brush data represents the basic building blocks used to 
create the geometry of the level. A brush is a convex, 3D shape composed of multiple brush sides 
(planes). Brushes are mainly used to define solid areas, such as walls, floors, and ceilings, 
or to specify special areas that trigger game events, such as doors or teleporters.

The Brush class stores the brush data:

-brushSide: This field represents the starting brush side (plane) for the brush. It is an index 
into the brush sides array that is part of the level data.
-numOfBrushSides: This field represents the number of brush sides (planes) that make up the brush. 
Together with brushSide, you can iterate through all the sides of a brush.
-textureID: This field represents the texture index for the brush. It is an index into the texture 
array that is part of the level data. This texture is used to determine the appearance of the brush 
and can also contain additional information, such as if the brush is solid or non-solid, or if it 
has special properties (e.g., slippery, bouncy).

Overall, the brush data is an essential part of defining the level's geometry, appearance, and 
functionality in the Quake 3 engine.
*/

namespace BSP {
    class Brush {
    public:
        // Accessor (getter) methods
        int getBrushSide() const { return brushSide; }
        int getNumOfBrushSides() const { return numOfBrushSides; }
        int getTextureID() const { return textureID; }

        // Mutator (setter) methods
        void setBrushSide(int newBrushSide) { brushSide = newBrushSide; }
        void setNumOfBrushSides(int newNumOfBrushSides) { numOfBrushSides = newNumOfBrushSides; }
        void setTextureID(int newTextureID) { textureID = newTextureID; }

        // Default constructor
        Brush() : brushSide(0), numOfBrushSides(0), textureID(0) {}

        // Constructor with parameters
        Brush(int newBrushSide, int newNumOfBrushSides, int newTextureID)
            : brushSide(newBrushSide), numOfBrushSides(newNumOfBrushSides), textureID(newTextureID) {}

        ~Brush() = default;

    private:
        int brushSide;            // The starting brush side for the brush
        int numOfBrushSides;      // Number of brush sides for the brush
        int textureID;            // The texture index for the brush
    };

    class Brushes : public BSP::Element<Brush> {
    public:
        void validate() override;
        void displayData() const override;
    };
}
