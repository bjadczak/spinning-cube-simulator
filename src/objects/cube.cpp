//
// Created by Bartosz Jadczak on 09/11/2024.
//

#include "cube.h"
Cube::Cube()
{
    std::vector<Vertex> vertices = {
        // Front face
        {{0, 0, 1},     {},     {0, 0, 1}}, // Bottom-left
        {{1, 0, 1},     {},     {0, 0, 1}}, // Bottom-right
        {{1, 1, 1},     {},     {0, 0, 1}}, // Top-right
        {{0, 1, 1},     {},     {0, 0, 1}}, // Top-left

        // Back face
        {{0, 0, 0},     {},     {0, 0, -1}}, // Bottom-left
        {{1, 0, 0},     {},     {0, 0, -1}}, // Bottom-right
        {{1, 1, 0},     {},     {0, 0, -1}}, // Top-right
        {{0, 1, 0},     {},     {0, 0, -1}}, // Top-left

        // Left face
        {{0, 0, 0},     {},     {-1, 0, 0}}, // Bottom-left
        {{0, 0, 1},     {},     {-1, 0, 0}}, // Bottom-right
        {{0, 1, 1},     {},     {-1, 0, 0}}, // Top-right
        {{0, 1, 0},     {},     {-1, 0, 0}}, // Top-left

        // Right face
        {{1, 0, 0},     {},     {1, 0, 0}}, // Bottom-left
        {{1, 0, 1},     {},     {1, 0, 0}}, // Bottom-right
        {{1, 1, 1},     {},     {1, 0, 0}}, // Top-right
        {{1, 1, 0},     {},     {1, 0, 0}}, // Top-left

        // Top face
        {{0, 1, 1},    {},     {0, 1, 0}}, // Bottom-left
        {{1, 1, 1},    {},     {0, 1, 0}}, // Bottom-right
        {{1, 1, 0},    {},     {0, 1, 0}}, // Top-right
        {{0, 1, 0},    {},     {0, 1, 0}}, // Top-left

        // Bottom face
        {{0, 0, 1},     {},     {0, -1, 0}}, // Bottom-left
        {{1, 0, 1},     {},     {0, -1, 0}}, // Bottom-right
        {{1, 0, 0},     {},     {0, -1, 0}}, // Top-right
        {{0, 0, 0},     {},     {0, -1, 0}}  // Top-left
    };
    std::vector<GLuint> indices = {
        // Front face
        0, 1, 2, 2, 3, 0,

        // Back face
        4, 6, 5, 6, 4, 7,

        // Left face (inverted)
        8, 9, 10, 10, 11, 8,

        // Right face (inverted)
        12, 15, 14, 14, 13, 12,

        // Top face
        16, 17, 18, 18, 19, 16,

        // Bottom face
        20, 22, 21, 22, 20, 23
    };
    objectMesh = std::make_unique<Mesh>(vertices, indices, GL_FILL, GL_TRIANGLES);
}

void Cube::render()
{
    objectMesh->Draw();
}
void Cube::update()
{

}