//
// Created by Bartosz Jadczak on 10/11/2024.
//

#include "line.h"
#include <ranges>
#include <algorithm>
#include <memory>

Line::Line()
{
    std::vector<Vertex> vertices{};
    std::vector<GLuint> indices{};
    lineMesh = std::make_unique<Mesh>(vertices, indices, GL_FILL, GL_LINES);
}

void Line::updatePoints(std::vector<glm::vec3> points)
{
    std::vector<Vertex> vertices{};
    std::ranges::transform(
        points,
        std::back_inserter(vertices),
        [](glm::vec3 point)
        {
            return Vertex{point, {}, {}};
        }
        );

    std::vector<GLuint> indices{};

    for(int i = 0; i < vertices.size() - 1; i++)
    {
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    lineMesh->Update(vertices, indices);
}

void Line::render()
{
    lineMesh->Draw();
}
