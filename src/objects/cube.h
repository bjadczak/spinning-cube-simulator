//
// Created by Bartosz Jadczak on 09/11/2024.
//

#ifndef CUBE_H
#define CUBE_H
#include <memory>
#include <glm/fwd.hpp>
#include <glm/matrix.hpp>

#include "../mesh/mesh.h"
#include "../shader/shaderProgram.h"


class Cube {
    std::unique_ptr<Mesh> objectMesh;
    std::unique_ptr<Mesh> axisMesh;



public:
    explicit Cube();

    void render(const ShaderProgram& shader, bool showCube, bool showDiagonal) const;

    void update();

    glm::vec3 axis;
    glm::mat4 model;
};



#endif //CUBE_H
