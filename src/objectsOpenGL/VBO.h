//
// Created by Bartek Jadczak on 03/03/2024.
//

#ifndef VBO_H
#define VBO_H

#include <vector>
#include "vertex.h"
#include "../glew_glfw.h"

class VBO
{
public:
    // Reference ID of the Vertex Buffer Object
    GLuint ID;
    // Constructor that generates a Vertex Buffer Object and links it to vertices
    explicit VBO(std::vector<Vertex>& vertices);

    // Binds the VBO
    void Bind();
    // Unbinds the VBO
    void Unbind();
    // Deletes the VBO
    void Delete();
};

#endif //VBO_H
