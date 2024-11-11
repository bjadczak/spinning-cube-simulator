//
// Created by Bartosz Jadczak on 10/11/2024.
//

#include "cubeSimulationThreads.h"

#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <glm/gtx/quaternion.hpp>
#include "glm/gtx/string_cast.hpp"
#include <glm/gtx/vector_angle.hpp> // For glm::angle



CubeSimulationWithThreads::CubeSimulationWithThreads() {
    cubeSize = 1;
    cubeDensity = 1;
    cubeTilt = 25.f;
    cubeAngleVelocity = 45.f;
    gravityOn = true;
    traceSize = 180;
    timeStepMs = 0.01;
    type = CubeSimulationType::THREAD;
    {
        glm::vec3 originalVector(1.0f, 1.0f, 1.0f);
        glm::vec3 targetVector(0.0f, 1.0f, 0.0f);

        originalVector = glm::normalize(originalVector);
        targetVector = glm::normalize(targetVector);

        rotationAxisToStraighten = glm::cross(originalVector, targetVector);

        rotationAxisToStraighten = glm::normalize(rotationAxisToStraighten);

        angleToStraighten = glm::angle(originalVector, targetVector);
    }

    line = std::make_unique<Line>();


    {
        std::vector<Vertex> arrow_vertices = {
            {{0.0f, 0.0f, 0.0f},        {0.0f, 0.0f}, {-1.0f, -1.0f, -1.0f}},   // Podstawa
            {{-1.0f, -1.0f, -1.0f},     {0.0f, 0.0f}, {-1.0f, -1.0f, -1.0f}},   // Czubek
            {{-0.9f, -0.9f, -0.95f},    {0.0f, 0.0f}, {-1.0f, -1.0f, -1.0f}},  // Prawe skrzydło
            {{-0.95f, -0.95f, -0.9f},   {0.0f, 0.0f}, {-1.0f, -1.0f, -1.0f}}, // Lewe skrzydło
        };
        std::vector<GLuint> indices = {
            0, 1,  // Main line
            1, 2,  // Right wing
            1, 3,  // Left wing
        };

        gravityVector = std::make_unique<Mesh>(arrow_vertices, indices, GL_FILL, GL_LINES);
    }

    {
        std::vector<Vertex> arrow_vertices = {
            {{1, -1, 0},     {},     {1, 1, 1}}, // Punkt A
            {{0, 1, -1},     {},     {1, 1, 1}}, // Punkt B
            {{-1, 1, 0},     {},     {1, 1, 1}}, // Punkt C
            {{0, -1, 1},     {},     {1, 1, 1}}  // Punkt D
        };
        std::vector<GLuint> indices = {
            0, 1, 2, 2, 3, 0,
        };

        gravityPlane = std::make_unique<Mesh>(arrow_vertices, indices, GL_FILL, GL_TRIANGLES);
    }


    reset();
}

void CubeSimulationWithThreads::reset() {
    const float cube2 = cubeSize*cubeSize;
    float itOff = - cube2 * cubeDensity / 4.f;
    float itDiag = 2.f * cube2 * cubeDensity / 3.f;

    {
        std::lock_guard<std::mutex> guard(dataMutex);

        inertiaTensor = {
            itDiag, itOff, itOff,
            itOff, itDiag, itOff,
            itOff, itOff, itDiag
        };
        inertiaTensorInverse = glm::inverse(inertiaTensor);

        Q = glm::quat(1,1,1,1);
        Q = glm::normalize(Q);
        const glm::vec3 axis = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
        const glm::quat rotation = glm::angleAxis(glm::radians(cubeTilt), axis);
        Q = rotation * Q;

        W = glm::vec3{1,1,1};
        W = glm::normalize(W)* cubeAngleVelocity;
    }

}

void CubeSimulationWithThreads::advanceByStep() {
    float delta_t = timeStepMs / 1000.f;
    float half_delta_t = 0.5f * delta_t;
    float sixth_delta_t = delta_t / 6.0f;
    glm::quat localQ;
    glm::vec3 localW;
    {
        std::lock_guard<std::mutex> guard(dataMutex);
        localQ = Q;
        localW = W;
    }

    // Calculate k1 step
    glm::vec3 k1_v = dW_dt(getExternalForce(localQ), localW);
    glm::quat q1 = dQ_dt(localQ, localW);

    // Calculate k2 step
    glm::vec3 W_k2 = localW + half_delta_t * k1_v;
    glm::vec3 k2_v = dW_dt(getExternalForce(localQ), W_k2);
    glm::quat q2 = dQ_dt(localQ + half_delta_t * q1, W_k2);

    // Calculate k3 step
    glm::vec3 W_k3 = localW + half_delta_t * k2_v;
    glm::vec3 k3_v = dW_dt(getExternalForce(localQ), W_k3);
    glm::quat q3 = dQ_dt(localQ + half_delta_t * q2, W_k3);

    // Calculate k4 step
    glm::vec3 W_k4 = W + delta_t * k3_v;
    glm::vec3 k4_v = dW_dt(getExternalForce(localQ), W_k4);
    glm::quat q4 = dQ_dt(localQ + delta_t * q3, W_k4);

    {
        std::lock_guard<std::mutex> guard(dataMutex);
        // Update W and Q using weighted sums of k1, k2, k3, k4
        W += sixth_delta_t * (k1_v + 2.0f * (k2_v + k3_v) + k4_v);
        Q += (q1 + 2.0f * (q2 + q3) + q4) * sixth_delta_t;
        Q = glm::normalize(Q);
    }

}

void CubeSimulationWithThreads::workerThread()
{
    static float time = std::chrono::system_clock::now().time_since_epoch().count();
    while(isRunning)
    {
        while (loopsToDo > 0)
        {
            --loopsToDo;
            advanceByStep();
            time+=timeStepMs;
        }
    }
}

void CubeSimulationWithThreads::startThread()
{
    worker = std::thread(&CubeSimulationWithThreads::workerThread, this);
    isRunning = true;
}
void CubeSimulationWithThreads::stopThread()
{
    isRunning = false;
    if(worker.joinable())
    {
        worker.join();
    }
}
void CubeSimulationWithThreads::addLoopsToDo(const int newLoops)
{
    loopsToDo += newLoops;
}

glm::vec3 CubeSimulationWithThreads::getExternalForce(const glm::quat Q) const {
    if(!gravityOn) return glm::vec3(0.0f);

    // Calculate middle point of cube
    const auto q = Q * glm::vec3(cubeSize) / 2.f;

    constexpr float gg = 9.81;

    // Define a gravity vector pointing diagonally downwards.
    // Normalize it, then scale it by the gravitational acceleration constant.
    auto g = glm::normalize(glm::vec3(-1, -1, -1)) * gg;

    // Apply a cross product to simulate torque-like effects from gravity on point q.
    g = glm::cross(q, g);

    // Rotate the gravity vector back into the cube's local space using quaternion math.
    // qN is the rotated version of gravity g as a quaternion.
    const glm::quat qN = glm::conjugate(Q) * glm::quat(0, g) * Q;

    return {qN.x, qN.y, qN.z};
}

void CubeSimulationWithThreads::updateTrace() {
    glm::vec3 q;
    {
        std::lock_guard<std::mutex> guard(dataMutex);
        q = Q * glm::vec3(cubeSize);
    }

    auto model = glm::identity<glm::mat4>();

    // Straighten up the cube
    model = glm::rotate(model, angleToStraighten, rotationAxisToStraighten);
    q = glm::vec3(model * glm::vec4(q, 1));
    tracePoints.push_back(q);
    while(tracePoints.size() > traceSize)
        tracePoints.erase(tracePoints.begin());
    line->updatePoints(tracePoints);
}

void CubeSimulationWithThreads::renderCube(const ShaderProgram& shader, const BaseCamera& camera, Cube& cube, bool showCube, bool showDiagonal)
{
    auto model = glm::identity<glm::mat4>();
    model = model * glm::scale(model, glm::vec3(cubeSize));

    // Straighten up the cube
    model = glm::rotate(model, angleToStraighten, rotationAxisToStraighten);

    {
        std::lock_guard<std::mutex> guard(dataMutex);
        model = model * glm::toMat4(Q);
    }
    shader.setUniform("model", model);
    shader.setUniform("projection", camera.getProjectionMatrix());
    shader.setUniform("view", camera.getViewMatrix());
    cube.render(shader, showCube, showDiagonal);
}

void CubeSimulationWithThreads::renderLine(const ShaderProgram &shader, const BaseCamera &camera, bool showLine) const
{
    if(!showLine) return;
    shader.setUniform("color", glm::vec3(1, 1, 1));
    shader.setUniform("opacity", 1.f);
    shader.setUniform("model", glm::mat4(1.0f));
    shader.setUniform("projection", camera.getProjectionMatrix());
    shader.setUniform("view", camera.getViewMatrix());
    line->render();
}

void CubeSimulationWithThreads::renderGravityLine(const ShaderProgram &shader, const BaseCamera &camera, bool showGravity)
{
    if(!showGravity) return;
    glm::vec3 q;
    {
        std::lock_guard<std::mutex> guard(dataMutex);
        q = Q * glm::vec3(cubeSize);
    }
    auto model = glm::identity<glm::mat4>();
    model = glm::rotate(model, angleToStraighten, rotationAxisToStraighten);
    model = glm::translate(model, q / 2.f);

    shader.setUniform("color", glm::vec3(1, 0, 0));
    shader.setUniform("opacity", 1.f);
    shader.setUniform("model", model);
    shader.setUniform("projection", camera.getProjectionMatrix());
    shader.setUniform("view", camera.getViewMatrix());
    gravityVector->Draw();

    model = glm::identity<glm::mat4>();

    // Straighten up the cube
    model = glm::rotate(model, angleToStraighten, rotationAxisToStraighten);

    shader.setUniform("model", model);
    shader.setUniform("color", glm::vec3(1, 1, 1));
    shader.setUniform("opacity", 0.2f);
    glDisable(GL_DEPTH_TEST);
    gravityPlane->Draw();
    glEnable(GL_DEPTH_TEST);
}


glm::vec3 CubeSimulationWithThreads::dW_dt(const glm::vec3 N, const glm::vec3 W) const
{
    return inertiaTensorInverse * (N + glm::cross(inertiaTensor * W, W) );
}

glm::quat CubeSimulationWithThreads::dQ_dt(const glm::quat Q, const glm::vec3 W) {
    return Q * glm::quat(0, W / 2.f);
}