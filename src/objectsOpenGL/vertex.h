//
// Created by Bartek Jadczak on 03/03/2024.
//

#ifndef VERTEX_H
#define VERTEX_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec2 texture;
    glm::vec3 color;
};

#endif //VERTEX_H
