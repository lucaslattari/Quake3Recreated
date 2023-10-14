#include "bezierPatches.h"

// Método de tesselação
void QuadraticPatch::tesselate(int tessellationLevel) {
    vertices.resize((tessellationLevel + 1) * (tessellationLevel + 1));

    // Cada quadrado tem 2 triângulos e cada triângulo tem 3 vértices
    int total_indices = tessellationLevel * tessellationLevel * 2 * 3;
    indices.getValues().resize(total_indices);

    trianglesPerRow.resize(tessellationLevel);
    rowIndices.getValues().resize(tessellationLevel);

    std::vector<Vec3f> tempPos(3);  // array temporário para armazenar os pontos intermediários
    std::vector<Vec4f> tempColor(3);  // array temporário para armazenar os pontos intermediários

    for (int uIdx = 0; uIdx <= tessellationLevel; uIdx++) {
        float u = (float)uIdx / tessellationLevel;
        float B0_u = ((1.0f - u) * (1.0f - u));
        float B1_u = ((1.0f - u) * u * 2);
        float B2_u = u * u;

        Vec3f auxPosition = controlPoints[0].getPosition() * B0_u 
            + controlPoints[3].getPosition() * B1_u
            + controlPoints[6].getPosition() * B2_u;

        // Interpolação de cores para uIdx
        Vec4f auxColor;
        for (int i = 0; i < 4; i++) {
            auxColor[i] = static_cast<float>(controlPoints[0].getColor()[i]) * B0_u
                + static_cast<float>(controlPoints[3].getColor()[i]) * B1_u
                + static_cast<float>(controlPoints[6].getColor()[i]) * B2_u;
        }

        vertices[uIdx].setPosition(auxPosition);
        vertices[uIdx].setColor(auxColor);
    }

    for (int vIdx = 1; vIdx <= tessellationLevel; vIdx++) {
        float v = (float)vIdx / tessellationLevel;
        float B0_v = ((1.0f - v) * (1.0f - v));
        float B1_v = ((1.0f - v) * v * 2);
        float B2_v = v * v;

        //position
        tempPos[0] = controlPoints[0].getPosition() * B0_v
            + controlPoints[1].getPosition() * B1_v
            + controlPoints[2].getPosition() * B2_v;

        tempPos[1] = controlPoints[3].getPosition() * B0_v
            + controlPoints[4].getPosition() * B1_v
            + controlPoints[5].getPosition() * B2_v;

        tempPos[2] = controlPoints[6].getPosition() * B0_v
            + controlPoints[7].getPosition() * B1_v
            + controlPoints[8].getPosition() * B2_v;

        // Interpolação de cores para vIdx (posições 0, 3, 6)
        for (int i = 0; i < 4; i++) {
            tempColor[0][i] = static_cast<float>(controlPoints[0].getColor()[i]) * B0_v
                + static_cast<float>(controlPoints[1].getColor()[i]) * B1_v
                + static_cast<float>(controlPoints[2].getColor()[i]) * B2_v;

            tempColor[1][i] = static_cast<float>(controlPoints[3].getColor()[i]) * B0_v
                + static_cast<float>(controlPoints[4].getColor()[i]) * B1_v
                + static_cast<float>(controlPoints[5].getColor()[i]) * B2_v;

            tempColor[2][i] = static_cast<float>(controlPoints[6].getColor()[i]) * B0_v
                + static_cast<float>(controlPoints[7].getColor()[i]) * B1_v
                + static_cast<float>(controlPoints[8].getColor()[i]) * B2_v;
        }

        for (int uIdx = 0; uIdx <= tessellationLevel; uIdx++) {
            float u = (float)uIdx / tessellationLevel;
            float B0_u = ((1.0f - u) * (1.0f - u));
            float B1_u = ((1.0f - u) * u * 2);
            float B2_u = u * u;

            Vec3f finalPosition = tempPos[0] * B0_u + tempPos[1] * B1_u + tempPos[2] * B2_u;
            vertices[vIdx * (tessellationLevel + 1) + uIdx].setPosition(finalPosition);

            Vec4f finalColor;
            for (int i = 0; i < 4; i++)
                finalColor[i] = tempColor[0][i] * B0_u + tempColor[1][i] * B1_u + tempColor[2][i] * B2_u;
            vertices[vIdx * (tessellationLevel + 1) + uIdx].setColor(finalColor);
        }
    }

    std::vector<int>& ind_values = indices.getValues();
    size_t index = 0;
    for (int row = 0; row < tessellationLevel; ++row) {
        for (int col = 0; col < tessellationLevel; ++col) {
            // Primeiro triângulo
            ind_values[index++] = row * (tessellationLevel + 1) + col;
            ind_values[index++] = (row + 1) * (tessellationLevel + 1) + col;
            ind_values[index++] = (row + 1) * (tessellationLevel + 1) + col + 1;

            // Segundo triângulo
            ind_values[index++] = row * (tessellationLevel + 1) + col;
            ind_values[index++] = (row + 1) * (tessellationLevel + 1) + col + 1;
            ind_values[index++] = row * (tessellationLevel + 1) + col + 1;
        }
    }
}

void QuadraticPatch::displayData() const {
    std::cout << "Vertices: " << vertices.size() << " vertices" << std::endl;
    std::cout << "Indices: " << indices.getValues().size() << " indices" << std::endl;
    std::cout << "Control Points: ";
    for (int i = 0; i < 9; ++i) {
        std::cout << "\nVertice " << i << ": " << controlPoints[i];
        if (i < 8) std::cout << ", ";
    }

    std::cout << "\nTesselated Vertices: ";
    for (int i = 0; i < vertices.size(); ++i) {
        std::cout << "\nVertice " << i << ": " << vertices[i];
        if (i < vertices.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;

    std::cout << "\nTesselated Indices: ";
    for (int i = 0; i < indices.getValues().size(); ++i) {
        std::cout << indices.getValues()[i];
        if (i < indices.getValues().size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;

    std::cout << "Triangles Per Row: " << trianglesPerRow.size() << " rows" << std::endl;

    // Exibindo o conteúdo de trianglesPerRow
    std::cout << "Triangles Per Row: ";
    for (size_t i = 0; i < trianglesPerRow.size(); ++i) {
        std::cout << trianglesPerRow[i];
        if (i < trianglesPerRow.size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;

    std::cout << "Row Indices: " << rowIndices.getValues().size() << " rows" << std::endl;
    // Exibindo o conteúdo de rowIndices
    std::cout << "Row Indices: ";
    for (size_t i = 0; i < rowIndices.getValues().size(); ++i) {
        std::cout << rowIndices.getValues()[i];
        if (i < rowIndices.getValues().size() - 1) std::cout << ", ";
    }
    std::cout << std::endl;
}

std::ostream& operator<<(std::ostream& os, const QuadraticPatch& patch) {
    patch.displayData();
    return os;
}

void PatchData::displayData() const {
    std::cout << "Face ID: " << faceId << std::endl;
    std::cout << "Texture ID: " << textureId << std::endl;
    std::cout << "Lightmap ID: " << lightmapId << std::endl;
    std::cout << "Width: " << width << std::endl;
    std::cout << "Height: " << height << std::endl;
    std::cout << "Quadratic Patches: " << quadraticPatches.size() << " patches" << std::endl;

    for (size_t i = 0; i < quadraticPatches.size(); ++i) {
        std::cout << "Patch " << i << ":" << std::endl;
        quadraticPatches[i].displayData();
    }
}

std::ostream& operator<<(std::ostream& os, const PatchData& patchData) {
    patchData.displayData();
    return os;
}