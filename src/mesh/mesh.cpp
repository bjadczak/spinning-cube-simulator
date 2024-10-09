//
// Created by Bartek Jadczak on 03/03/2024.
//

#include "mesh.h"
#include "../objectsOpenGL/EBO.h"

Mesh::Mesh (
        std::vector<Vertex> &vertices,
        std::vector<GLuint> &indices,
        GLenum glPolygonModeSet,
        GLenum glDrawElementMode
        )
: glPolygonModeSet(glPolygonModeSet), glDrawElementMode(glDrawElementMode)
{
    Mesh::vertices = vertices;
    Mesh::indices = indices;

    mVAO.Bind();
    // Generates Vertex Buffer Object and links it to vertices
    VBO mVBO(vertices);
    // Generates Element Buffer Object and links it to indices
    EBO mEBO(indices);
    // Links VBO attributes such as coordinates and colors to VAO
    mVAO.LinkAttrib(mVBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    mVAO.LinkAttrib(mVBO, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
    mVAO.LinkAttrib(mVBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(5 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    mVAO.Unbind();
    mVBO.Unbind();
    mEBO.Unbind();
}

void Mesh::Draw()
{
    mVAO.Bind();

    // Set glMode
    glPolygonMode(GL_FRONT_AND_BACK, glPolygonModeSet);

    // Draw the actual mesh
    glDrawElements(glDrawElementMode, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::Update (std::vector<Vertex> &vertices, std::vector<GLuint> &indices)
{
    Mesh::vertices = vertices;
    Mesh::indices = indices;

    mVAO.Bind();
    // Generates Vertex Buffer Object and links it to vertices
    VBO mVBO(vertices);
    // Generates Element Buffer Object and links it to indices
    EBO mEBO(indices);
    // Links VBO attributes such as coordinates and colors to VAO
    mVAO.LinkAttrib(mVBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
    mVAO.LinkAttrib(mVBO, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
    mVAO.LinkAttrib(mVBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(5 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    mVAO.Unbind();
    mVBO.Unbind();
    mEBO.Unbind();
}