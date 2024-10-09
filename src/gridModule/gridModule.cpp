//
// Created by Bartosz Jadczak on 09/10/2024.
//

#include "gridModule.h"
#include "../objectsOpenGL/vertex.h"

//TODO Implement
GridModule::GridModule (AppContext &appContext) :
    appContext(appContext),
    shader(std::make_unique<ShaderProgram>("res/shaders/gridModule/grid.vert", "res/shaders/gridModule/grid.frag"))
{
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    vertices.push_back(Vertex{glm::vec3(1, 1, 0), glm::vec3(0, 1 ,0)});
    vertices.push_back(Vertex{glm::vec3(1, -1, 0), glm::vec3(0, 1 ,0)});
    vertices.push_back(Vertex{glm::vec3(-1, -1, 0), glm::vec3(0, 1 ,0)});
    vertices.push_back(Vertex{glm::vec3(-1, 1, 0), glm::vec3(0, 1 ,0)});
    addQuad(indices, 0, 1, 2, 3);

    objectMesh = std::make_unique<Mesh>(Mesh(vertices, indices, GL_FILL, GL_TRIANGLES));

    // TODO Add logging of creation
}

void GridModule::draw () const {
    shader->Activate();

    shader->setUniform("near", appContext.camera->getNearPlane());
    shader->setUniform("far", appContext.camera->getFarPlane());
    shader->setUniform("view", appContext.camera->getViewMatrix());
    shader->setUniform("proj", appContext.camera->getProjectionMatrix());

    objectMesh->Draw();
}

void GridModule::destroy () const {
    shader->Delete();
}
