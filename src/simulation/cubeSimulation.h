//
// Created by Bartosz Jadczak on 10/11/2024.
//

#ifndef CUBESIMULATION_H
#define CUBESIMULATION_H
#include <glm/fwd.hpp>
#include <glm/detail/type_quat.hpp>

#include "cubeSimulationBase.h"
#include "../objects/line.h"

class CubeSimulation : public CubeSimulationBase {
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

    [[nodiscard]] glm::vec3 dW_dt(glm::vec3 N, glm::vec3 W) const;
    static glm::quat dQ_dt(glm::quat Q, glm::vec3 W);
    [[nodiscard]] glm::vec3 getExternalForce(glm::quat Q) const;

    void advanceByStep();

public:
    CubeSimulation();

    void reset() override;

    void updateTrace() override;

    void startThread() override {};
    void stopThread() override {};
    void addLoopsToDo(int newLoops) override;

    void renderCube(const ShaderProgram& shader, const BaseCamera& camera, Cube& cube, bool showCube, bool showDiagonal) override;
    void renderLine(const ShaderProgram& shader, const BaseCamera& camera, bool showLine) const override;
    void renderGravityLine(const ShaderProgram& shader, const BaseCamera& camera, bool showGravity) override;

};



#endif //CUBESIMULATION_H
