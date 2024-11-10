//
// Created by Bartosz Jadczak on 10/11/2024.
//

#ifndef CUBESIMULATION_H
#define CUBESIMULATION_H
#include <glm/fwd.hpp>
#include <glm/detail/type_quat.hpp>

#include "../camera/baseCamera.h"
#include "../shader/shaderProgram.h"
#include "../objects/cube.h"
#include "../objects/line.h"

class CubeSimulation {
    // Orientation
    glm::quat Q{};
    // Angle velocity
    glm::vec3 W{};
    glm::mat3 inertiaTensor{};
    glm::mat3 inertiaTensorInverse{};

    std::vector<glm::vec3> tracePoints;

    std::unique_ptr<Line> line;
    std::unique_ptr<Mesh> gravityVector;
    std::unique_ptr<Mesh> gravityPlane;

    glm::vec3 rotationAxisToStraighten{};
    float angleToStraighten;

public:
    float cubeSize;
    float cubeDensity;
    float cubeTilt;
    float cubeAngleVelocity;
    float timeStepMs;
    bool gravityOn;
    int traceSize;
    CubeSimulation();

    void reset();

    void advanceByStep();

    void updateTrace();

    void renderCube(ShaderProgram& shader, const BaseCamera& camera, Cube& cube) const;
    void renderLine(ShaderProgram& shader, const BaseCamera& camera) const;
    void renderGravityLine(ShaderProgram& shader, const BaseCamera& camera) const;

    glm::vec3 dW_dt(glm::vec3 N, glm::vec3 W) const;
    static glm::quat dQ_dt(glm::quat Q, glm::vec3 W);
    glm::vec3 getExternalForce(glm::quat Q) const;
};



#endif //CUBESIMULATION_H
