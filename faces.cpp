#include "Faces.h"
#include "utils.h"

namespace BSP {
    void Faces::validate() {
        for (const auto& face : elements) {
            warning_assert(face.getTextureID() >= 0, "TextureID is negative.");

            warning_assert(face.getEffect() >= -1, "Effect is less than -1.");

            warning_assert(face.getType() >= 1 && face.getType() <= 4, "Face type is out of the valid range.");

            warning_assert(face.getStartVertIndex() >= 0, "startVertIndex is negative.");
            warning_assert(face.getNumOfVerts() >= 0, "numOfVerts is negative.");
            warning_assert(face.getStartIndex() >= 0, "startIndex is negative.");
            warning_assert(face.getNumOfIndices() >= 0, "numOfIndices is negative.");

            warning_assert(face.getLightmapID() >= 0, "LightmapID is negative.");

            warning_assert(face.getLightMapCorner()[0] >= 0 && face.getLightMapCorner()[1] >= 0,
                "Invalid lightMapCorner values.");
            warning_assert(face.getLightMapSize()[0] >= 0 && face.getLightMapSize()[1] >= 0,
                "Invalid lightMapSize values.");

            warning_assert(face.getSize()[0] >= 0 && face.getSize()[1] >= 0, "Invalid face size values.");

            warning_assert(face.getNormal().length() <= 1.0f, "Normal vector magnitude is greater than 1.0f.");

            for (int i = 0; i < 2; ++i) {
                float vecMagnitude = std::sqrt(face.getLightMapVecs()[i].x() * face.getLightMapVecs()[i].x() +
                    face.getLightMapVecs()[i].y() * face.getLightMapVecs()[i].y() +
                    face.getLightMapVecs()[i].z() * face.getLightMapVecs()[i].z());
                warning_assert(vecMagnitude <= 1.0f,
                    "LightMapVecs s and t unit vectors magnitude is greater than 1.0f.");
            }
        }
    }

    void Faces::displayData() const {
        for (const auto& face : elements) {
            // Display face data using getter methods
            // Add your display code here
        }
    }

}