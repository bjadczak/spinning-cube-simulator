//
// Created by Bartosz Jadczak on 22/10/2024.
//

#include "millingHead.h"
#include <glm/vec3.hpp>

MillingHead::MillingHead()
{
    type = Spherical;
    height = 10;
    radius = 10;
    generateMillingHead();
}
void MillingHead::setType(MillType type) { this->type = type; }
void MillingHead::setHeight(float height) { this->height = height; }
void MillingHead::setRadius(float radius) { this->radius = radius; }
void MillingHead::generateMillingHead()
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    generateCylinder(radius, height, 16, type, vertices, indices);
    cylinderMesh = std::make_unique<Mesh>(Mesh(vertices, indices, GL_FILL, GL_TRIANGLES));
}

void MillingHead::generateCylinder(
    float radius, float height, int segments, MillType type,
    std::vector<Vertex>& vertices,
    std::vector<unsigned int>& indices)
{


    float radiusForHeight = type == Spherical ? radius : 0;
    height += radiusForHeight;

    for (double i = 0.0; i < 2 * M_PI; i += M_PI / 16.0) {
        glm::vec3 point1 = glm::vec3(radius * cos(i), radiusForHeight, radius * sin(i));
        glm::vec3 point2 = glm::vec3(radius * cos(i), height, radius * sin(i));


        vertices.push_back(Vertex{point1, {}, glm::vec3(1.0f, 0.0f, 0.0f)});
        vertices.push_back(Vertex{point2, {}, glm::vec3(1.0f, 0.0f, 0.0f)});
    }

    int all = (2.0 * M_PI) / (M_PI / 16.0);

    for (int i = 0; i < all; i += 1) {
        indices.push_back(2 * i);
        indices.push_back((2 * i + 1) % (2 * all));
        indices.push_back((2 * i + 2) % (2 * all));

        indices.push_back((2 * i + 1) % (2 * all));
        indices.push_back((2 * i + 2) % (2 * all));
        indices.push_back((2 * i + 3) % (2 * all));
    }

    int parallelCount = 128; int meridianCount = segments;

    // add top vertex
    vertices.push_back(Vertex{glm::vec3(0, radius * 2, 0), {}, glm::vec3(1.0f, 0.0f, 0.0f)});

    // generate vertices per stack / slice
    for (int i = 0; i < parallelCount - 1; i++) {
        auto phi = M_PI * double(i + 1) / double(parallelCount);
        for (int j = 0; j < meridianCount; j++) {
            auto theta = 2.0 * M_PI * double(j) / double(meridianCount);
            auto x = std::sin(phi) * std::cos(theta) * radius;
            auto y = std::cos(phi) * radius + radius;
            auto z = std::sin(phi) * std::sin(theta) * radius;
            auto pos = glm::vec3(x, y, z);
            vertices.push_back(Vertex{pos, {}, glm::vec3(1.0f, 0.0f, 0.0f)});
        }
    }

    if(type == Spherical)
    {
        // add bottom vertex
        vertices.push_back(Vertex{glm::vec3(0, radius, 0), {}, glm::vec3(1.0f, 0.0f, 0.0f)});


        for (int i = 0; i < meridianCount; ++i) {
            auto i0 = i + 1;
            auto i1 = (i + 1) % meridianCount + 1;
            indices.push_back(0);
            indices.push_back(i0);
            indices.push_back(i1);
            i0 = i + meridianCount * (parallelCount - 2) + 1;
            i1 = (i + 1) % meridianCount + meridianCount * (parallelCount - 2) + 1;
            indices.push_back(vertices.size() - 1);
            indices.push_back(i0);
            indices.push_back(i1);
        }

        for (int j = 0; j < parallelCount - 2; j++) {
            auto j0 = j * meridianCount + 1;
            auto j1 = (j + 1) * meridianCount + 1;
            for (int i = 0; i < meridianCount; i++) {
                auto i0 = j0 + i;
                auto i1 = j0 + (i + 1) % meridianCount;
                auto i2 = j1 + (i + 1) % meridianCount;
                auto i3 = j1 + i;

                indices.push_back(i0);
                indices.push_back(i1);
                indices.push_back(i2);

                indices.push_back(i3);
                indices.push_back(i0);
                indices.push_back(i2);
            }
        }
    }
}

void MillingHead::render()
{
    cylinderMesh->Draw();
}
