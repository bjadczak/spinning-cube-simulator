//
// Created by Bartosz Jadczak on 11/11/2024.
//

#ifndef CUBESIMULATIONBASE_H
#define CUBESIMULATIONBASE_H

#include "../camera/baseCamera.h"
#include "../shader/shaderProgram.h"
#include "../objects/cube.h"

enum CubeSimulationType
{
    THREAD,
    NONTHREAD
};

class CubeSimulationBase {
public:
    float cubeSize;
    float cubeDensity;
    float cubeTilt;
    float cubeAngleVelocity;
    float timeStepMs;
    bool gravityOn;
    int traceSize;

    CubeSimulationType type;

    virtual void reset() = 0;

    virtual void startThread() = 0;
    virtual void stopThread() = 0;
    virtual void addLoopsToDo(int newLoops) = 0;

    virtual void updateTrace() = 0;

    virtual void renderCube(const ShaderProgram& shader, const BaseCamera& camera, Cube& cube, bool showCube, bool showDiagonal) = 0;
    virtual void renderLine(const ShaderProgram& shader, const BaseCamera& camera, bool showLine) const = 0;
    virtual void renderGravityLine(const ShaderProgram& shader, const BaseCamera& camera, bool showGravity) = 0;
};



#endif //CUBESIMULATIONBASE_H
