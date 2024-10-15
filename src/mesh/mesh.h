//
// Created by Bartek Jadczak on 03/03/2024.
//

#ifndef OPENGLMODELEDITOR_MESH_H
#define OPENGLMODELEDITOR_MESH_H

#include "../glew_glfw.h"
#include "../objectsOpenGL/vertex.h"
#include "../objectsOpenGL/VBO.h"
#include "../objectsOpenGL/VAO.h"

class Mesh
{
public:
    std::vector <Vertex> vertices;
    std::vector <GLuint> indices;
    // Store VAO in public so it can be used in the Draw function
    VAO mVAO;

    GLenum glPolygonModeSet;
    GLenum glDrawElementMode;

    // Initializes the mesh
    Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, GLenum glPolygonModeSet, GLenum glDrawElementMode);

    void Update(std::vector <Vertex>& vertices, std::vector <GLuint>& indices);

    // Draws the mesh
    void Draw(int instanceCount = 1);
};


#endif //OPENGLMODELEDITOR_MESH_H
