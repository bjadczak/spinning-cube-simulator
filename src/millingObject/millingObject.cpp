//
// Created by Bartosz Jadczak on 09/10/2024.
//

#include "millingObject.h"

#include "../QuadUtils.h"

void millingObject::generateMesh() {
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    vertices.push_back(Vertex{glm::vec3(width / 2, 0, depth / 2), glm::vec3(0, 1 ,0)});
    vertices.push_back(Vertex{glm::vec3(width / 2, 0, - depth / 2), glm::vec3(0, 1 ,0)});
    vertices.push_back(Vertex{glm::vec3(- width / 2, 0, - depth / 2), glm::vec3(0, 1 ,0)});
    vertices.push_back(Vertex{glm::vec3(- width / 2, 0, depth / 2), glm::vec3(0, 1 ,0)});
    addQuad(indices, 0, 1, 2, 3);

    if(objectMesh != nullptr)
    {
        objectMesh->Update(vertices, indices);
    }
    else
    {
        objectMesh = std::make_unique<Mesh>(Mesh(vertices, indices, GL_FILL, GL_TRIANGLES));
    }
}

millingObject::millingObject(): width(15), depth(15), height(5) {
    generateMesh();
}
