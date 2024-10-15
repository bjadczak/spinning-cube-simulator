//
// Created by Bartosz Jadczak on 18/10/2024.
//

#ifndef MILL_H
#define MILL_H

#include "../shader/shaderProgram.h"

#include <vector>
#include <atomic>
#include <expected>
#include <mutex>

#include "millingHead.h"


class Mill {
    MillType type;
    float height;
    float radius;
    glm::vec3 position{};
    float velocity;
    float maxDescendAngle;
    float minHeight;
    float tMillPath;

    int currentPathPoint{};
    std::vector<glm::vec3> path;

    std::atomic<bool> threadRunning;
    std::atomic<bool> threadFinished;
    std::atomic<bool> stopThreadSignal;
    std::atomic<bool> threadError;

    std::mutex heightMapMutex;

    std::unique_ptr<MillingHead> head;


public:
    Mill(float height, float radius, glm::vec3 position, float velocity, float minAngleDescend, float minHeight);

    void render(ShaderProgram& shaderProgram);

    void setPath(std::vector<glm::vec3> newPath);
    void setType(MillType newType);
    void setHeight(float newHeight);
    void setRadius(float newRadius);
    void setVelocity(float newVelocity);
    void setPosition(glm::vec3 newPosition);

    [[nodiscard]] std::vector<glm::vec3> getPath() const;
    bool isThreadRunning() const;
    bool isThreadFinished() const;
    float getVelocity() const;
    MillType getType() const;
    float getRadius() const;
    float getHeight() const;

    void startInstant(std::vector<std::vector<float>> &heightMap, glm::vec3 baseDimensions);
    void instantThread(std::vector<std::vector<float>> &heightMap, glm::vec3 baseDimensions);

    void millThread(std::vector<std::vector<float>> &heightMap, glm::vec3 baseDimensions);
    std::expected<void, std::string> advance(std::vector<std::vector<float>> &heightMap, glm::vec3 baseDimensions, float deltaTime);
    void startMilling(std::vector<std::vector<float>> &heightMap, glm::vec3 baseDimensions);

    void signalStop();
    bool pathFinished() const;
    void reset();

    std::expected<void, std::string> mill(std::vector<std::vector<float>> &heightMap, glm::vec3 baseDimensions, glm::vec3 p1, glm::vec3 p2);
    static int getGridColumnIndex(float xCoord, float baseWidth, int mapWidth);
    static int getGridRowIndex(float zCoord, float baseDepth, int mapDepth);
    static int getGridRadius(float radius, float baseDimension, int mapSize);
    static bool isWithinBounds(int col, int row, const std::vector<std::vector<float>> &heightMap);
    static glm::vec3 getGridPosition(int col, int row, glm::vec3 baseDimensions, const std::vector<std::vector<float>> &heightMap);
    static float computeSquaredDistance(float x1, float x2, float z1, float z2);
    static float computeMilledHeight(float pointRadius2, float radius, float startHeight, MillType type);
    void updateHeightMap(std::vector<std::vector<float>> &heightMap, int col, int row, float newHeight);
};



#endif //MILL_H
