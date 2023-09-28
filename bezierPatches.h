#pragma once

#include <iostream>
#include <vector>

#include "vertices.h"

class QuadraticPatch {
private:
    std::vector<BSP::Vertex> vertices;  // Vetor de v�rtices
    std::vector<GLuint> indices;  // Vetor de �ndices
    BSP::Vertex controlPoints[9];  // Pontos de controle para interpola��o
    std::vector<int> trianglesPerRow;  // Tri�ngulos por linha
    std::vector<std::vector<unsigned int>> rowIndices;  // �ndices por linha

public:
    // M�todos para acessar e modificar os membros, se necess�rio
    const std::vector<BSP::Vertex>& getVertices() const { return vertices; }
    std::vector<BSP::Vertex>& getVertices() { return vertices; }
    const std::vector<GLuint>& getIndices() const { return indices; }
    std::vector<GLuint>& getIndices() { return indices; }
    const BSP::Vertex* getControlPoints() const { return controlPoints; }
    BSP::Vertex* getControlPoints() { return controlPoints; }
    const std::vector<int>& getTrianglesPerRow() const { return trianglesPerRow; }
    std::vector<int>& getTrianglesPerRow() { return trianglesPerRow; }
    const std::vector<std::vector<unsigned int>>& getRowIndices() const { return rowIndices; }
    std::vector<std::vector<unsigned int>>& getRowIndices() { return rowIndices; }

    // M�todo de tessela��o
    void Tesselate() {
        // ... c�digo de tessela��o
    }

    // M�todo de desenho
    void Draw() {
        // ... c�digo de desenho
    }

    // ... outros m�todos e propriedades necess�rias
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

    // Setters
    void setFaceId(int faceId) { this->faceId = faceId; }
    void setTextureId(int textureId) { this->textureId = textureId; }
    void setLightmapId(int lightmapId) { this->lightmapId = lightmapId; }
    void setWidth(int width) { this->width = width; }
    void setHeight(int height) { this->height = height; }
    void setQuadraticPatches(const std::vector<QuadraticPatch>& quadraticPatches) { this->quadraticPatches = quadraticPatches; }

    // M�todo para obter o n�mero de patches quadr�ticos
    int getNumQuadraticPatches() const {
        return quadraticPatches.size();
    }

    void resizeQuadraticPatches(int new_size) {
        quadraticPatches.resize(new_size);
    }
};
