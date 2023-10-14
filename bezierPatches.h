#pragma once

#include <iostream>
#include <vector>
#include "indexedData.h"
#include "vertices.h"

class QuadraticPatch {
private:
    std::vector<BSP::Vertex> vertices;  // Vetor de vértices
    IndexedData indices;  // Vetor de índices
    BSP::Vertex controlPoints[9];  // Pontos de controle para interpolação
    std::vector<int> trianglesPerRow;  // Triângulos por linha
    IndexedData rowIndices;  // Índices por linha

public:
    // Métodos para acessar e modificar os membros, se necessário
    const std::vector<BSP::Vertex>& getVertices() const { return vertices; }
    std::vector<BSP::Vertex>& getVertices() { return vertices; }
    const IndexedData& getIndices() const { return indices; }
    IndexedData& getIndices() { return indices; }
    const BSP::Vertex* getControlPoints() const { return controlPoints; }
    BSP::Vertex* getControlPoints() { return controlPoints; }
    const std::vector<int>& getTrianglesPerRow() const { return trianglesPerRow; }
    std::vector<int>& getTrianglesPerRow() { return trianglesPerRow; }
    const IndexedData& getRowIndices() const { return rowIndices; }
    IndexedData& getRowIndices() { return rowIndices; }
    friend std::ostream& operator<<(std::ostream& os, const QuadraticPatch& patch);

    // Método de tesselação
    void tesselate(int tessellationLevel);
    void displayData() const;
};

class PatchData {
private:
    int faceId;
    int textureId;
    int lightmapId;
    int width;
    int height;
    std::vector<QuadraticPatch> quadraticPatches;

public:
    // Construtor
    PatchData(int faceId, int textureId, int lightmapId, int width, int height)
        : faceId(faceId), textureId(textureId), lightmapId(lightmapId), width(width), height(height) {}

    // Getters
    int getFaceId() const { return faceId; }
    int getTextureId() const { return textureId; }
    int getLightmapId() const { return lightmapId; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    const std::vector<QuadraticPatch>& getQuadraticPatches() const { return quadraticPatches; }
    std::vector<QuadraticPatch>& getQuadraticPatches() { return quadraticPatches; }
    
    // Setters
    void setFaceId(int faceId) { this->faceId = faceId; }
    void setTextureId(int textureId) { this->textureId = textureId; }
    void setLightmapId(int lightmapId) { this->lightmapId = lightmapId; }
    void setWidth(int width) { this->width = width; }
    void setHeight(int height) { this->height = height; }
    void setQuadraticPatches(const std::vector<QuadraticPatch>& quadraticPatches) { this->quadraticPatches = quadraticPatches; }

    // Método para obter o número de patches quadráticos
    int getNumQuadraticPatches() const {
        return quadraticPatches.size();
    }

    void resizeQuadraticPatches(int new_size) {
        quadraticPatches.resize(new_size);
    }

    friend std::ostream& operator<<(std::ostream& os, const PatchData& patchData);

    void displayData() const;
};
