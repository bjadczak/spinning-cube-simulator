//
// Created by Bartosz Jadczak on 09/10/2024.
//

#ifndef MILLINGOBJECT_H
#define MILLINGOBJECT_H

#include <memory>
#include "../mesh/mesh.h"
#include "../texture/texture.h"

class MillingPlate {


    glm::vec<2, int> heightMapSize;


    std::unique_ptr<Mesh> objectMesh;
    float maxValue = 0;



public:
    std::unique_ptr<Texture> heightMap;
    std::vector<std::vector<float>> heightMapData;
    explicit MillingPlate(glm::vec<2, int> size, float defaultValue);

    void generateMap(glm::vec<2, int> size, float defaultValue);

    void render(int instanceCount);

    void update();

};



#endif //MILLINGOBJECT_H
