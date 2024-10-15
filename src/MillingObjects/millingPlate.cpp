//
// Created by Bartosz Jadczak on 09/10/2024.
//

#include "millingPlate.h"

#include "../QuadUtils.h"

void MillingPlate::generateMap(glm::vec<2, int> size, float defaultValue) {
    maxValue = defaultValue;
    heightMapData.clear();
    heightMapSize = size;
    for(int i = 0; i < heightMapSize.x; i++) {
        heightMapData.emplace_back();
        for(int j = 0; j < heightMapSize.y; j++)
            heightMapData.back().push_back(defaultValue);
    }

    std::vector<float> t(heightMapSize.x*heightMapSize.y);
    if(heightMap)
    {
        heightMap.reset();
    }
    heightMap = std::make_unique<Texture>(heightMapSize.x, heightMapSize.y, 1, GL_RED, GL_RED, GL_FLOAT, GL_TEXTURE_2D,
                                                     nullptr);
    for(int i = 0; i < heightMapSize.x; i++)
        for(int j = 0; j < heightMapSize.y; j++)
            t[j*heightMapSize.x + i] = heightMapData[i][j]/defaultValue;
    heightMap->update2D(t.data());

}

MillingPlate::MillingPlate(glm::vec<2, int> size, float defaultValue) {

    generateMap(size, defaultValue);
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    vertices.push_back(Vertex{});
    objectMesh = std::make_unique<Mesh>(Mesh(vertices, indices, GL_FILL, GL_POINTS));

}

void MillingPlate::render(int instanceCount) {
    objectMesh->Draw(instanceCount);
}

void MillingPlate::update() {
    std::vector<float> t(heightMapSize.x*heightMapSize.y);
    for(int i = 0; i < heightMapSize.x; i++)
        for(int j = 0; j < heightMapSize.y; j++)
            t[j*heightMapSize.x + i] = heightMapData[i][j]/maxValue;
    heightMap->update2D(t.data());
}
