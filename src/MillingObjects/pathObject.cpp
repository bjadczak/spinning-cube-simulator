//
// Created by Bartosz Jadczak on 15/10/2024.
//

#include "pathObject.h"
#include <algorithm>
#include <iterator>

void PathObject::generateMesh(const std::vector<glm::vec3> &path)
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::ranges::transform(path, std::back_inserter(vertices),
                           [](const glm::vec3 v){ return Vertex{v, {}, {0.f, 1.f, 0.f}};});
    if(objectMesh != nullptr) {
        objectMesh.reset();
    }
    objectMesh = std::make_unique<Mesh>(Mesh(vertices, indices, GL_FILL, GL_LINE_STRIP));
}

PathObject::PathObject(const std::vector<glm::vec3> &path)
{
    generateMesh(path);
}

void PathObject::render() const
{
    objectMesh->Draw();
}
