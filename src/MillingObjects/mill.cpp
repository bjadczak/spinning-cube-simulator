//
// Created by Bartosz Jadczak on 18/10/2024.
//

#include <thread>
#include <iostream>
#include "mill.h"
#include "../alghorithm/Bresenham.h"

Mill::Mill(float height, float radius, glm::vec3 position, float velocity, float minAngleDescend, float minHeight)
{
    type = Spherical;
    this->height = height;
    this->radius = radius;
    this->position = position;
    this->velocity = velocity;
    this->maxDescendAngle = minAngleDescend;
    this->minHeight = minHeight;
    tMillPath = 0;
    head = std::make_unique<MillingHead>();
    position = glm::vec3(0, 200, 0);
}

void Mill::render(ShaderProgram& shaderProgram)
{
    shaderProgram.setUniform("model", glm::translate(glm::mat4(1.0f), position));
    head->render();
}

void Mill::setPath(std::vector<glm::vec3> newPath)
{
    path = std::move(newPath);
    currentPathPoint = 0;
    tMillPath = 0;
}
void Mill::setType(MillType newType) { type = newType; head->setType(newType); head->generateMillingHead(); }
void Mill::setHeight(float newHeight) { height = newHeight; head->setHeight(newHeight); head->generateMillingHead(); }
void Mill::setRadius(float newRadius) { radius = newRadius; head->setRadius(newRadius); head->generateMillingHead(); }
void Mill::setVelocity(float newVelocity) { velocity = newVelocity; }
void Mill::setPosition(glm::vec3 newPosition) { position = newPosition; }

std::vector<glm::vec3> Mill::getPath() const { return path; }
bool Mill::isThreadRunning() const { return threadRunning; }
bool Mill::isThreadFinished() const { return threadFinished; }
float Mill::getVelocity() const { return velocity; }
MillType Mill::getType() const { return type; }
float Mill::getRadius() const { return radius; }
float Mill::getHeight() const { return height; }

void Mill::startInstant(std::vector<std::vector<float> > &heightMap, glm::vec3 baseDimensions)
{
    threadRunning = true;
    threadFinished = false;
    stopThreadSignal = false;
    threadError = false;
    std::thread threadObj(&Mill::instantThread, this, std::ref(heightMap), baseDimensions);
    threadObj.detach();
}

void Mill::startMilling(std::vector<std::vector<float>> &heightMap, glm::vec3 baseDimensions)
{
    threadRunning = true;
    threadFinished = false;
    stopThreadSignal = false;
    threadError = false;
    std::thread threadObj(&Mill::millThread, this, std::ref(heightMap), baseDimensions);
    threadObj.detach();
}

bool Mill::pathFinished() const { return !path.empty() && currentPathPoint == path.size() - 1; }

void Mill::reset()
{
    signalStop();
    currentPathPoint = 0;
    tMillPath = 0;
    position = glm::vec3(0, 0, 0);
}

void Mill::signalStop() { stopThreadSignal = true; }

void Mill::millThread(std::vector<std::vector<float>> &heightMap, glm::vec3 baseDimensions)
{
    float deltaTime = 10/1000.f;
    while(!stopThreadSignal && currentPathPoint != path.size() - 1) {
        auto e = advance(heightMap, baseDimensions, deltaTime);
        if(!e) {
            threadError = true;
            std::cerr << e.error() << std::endl;
            break;
        }
        //setPosition(path[currentPathPoint + 1]);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    threadFinished = true;
    threadRunning = false;
}

std::expected<void, std::string> Mill::advance(std::vector<std::vector<float>> &heightMap, glm::vec3 baseDimensions, float deltaTime)
{
    if (!path.empty() && currentPathPoint != path.size() - 1) {
        float amountLeft = velocity * deltaTime;
        while (amountLeft > 0) {
            int currId = currentPathPoint;
            glm::vec3 currPoint = path[currId];
            glm::vec3 nextPoint = path[currId + 1];
            glm::vec3 direction = nextPoint - currPoint;
            float distance = glm::length(direction);
            float tIncrement = amountLeft / distance;
            float t = tMillPath;
            if (t + tIncrement > 1) {
                auto e = mill(heightMap,baseDimensions, currPoint + t * direction, nextPoint);
                if(!e) return e;
                currentPathPoint++;
                tMillPath = 0;
                if(currentPathPoint == path.size()-1) {
                    setPosition(nextPoint);
                    break;
                }
                amountLeft -= distance;
                continue;
            }
            amountLeft = 0;
            tMillPath += tIncrement;
            auto e = mill(heightMap, baseDimensions, currPoint + t * direction, currPoint + tMillPath * direction);
            if(!e) return e;

            setPosition(currPoint + tMillPath * direction);

        }
    }
    return {};
}

void Mill::instantThread(std::vector<std::vector<float> > &heightMap, glm::vec3 baseDimensions)
{
    for (int i = currentPathPoint; i < path.size() - 1 && !stopThreadSignal; i++)
    {
        auto e = mill(heightMap, baseDimensions, path[currentPathPoint], path[currentPathPoint + 1]);
        if (!e)
        {
            threadError = true;
            std::cerr << e.error() << std::endl;
            break;
        }
        currentPathPoint++;
    }
    threadFinished = true;
    threadRunning = false;
}

std::expected<void, std::string>
Mill::mill(std::vector<std::vector<float> > &heightMap, glm::vec3 baseDimensions, glm::vec3 startPos, glm::vec3 endPos)
{
    // Calculate the milling direction
    glm::vec3 millingDirection = endPos - startPos;

    // Store the list of starting points for milling
    std::vector<glm::vec<2, int> > startingPoints;

    // Check if the milling tool crosses the minimum allowed height
    if (endPos.y < minHeight) { return std::unexpected("Mill has crossed min height limit"); }

    // Calculate the descend angle based on vertical direction
    float descendAngle = glm::dot(glm::normalize(millingDirection), glm::vec3(0, -1, 0));

    // Convert 3D positions to 2D grid indices on the height map
    int startPosCol = getGridColumnIndex(startPos.x, baseDimensions.x, heightMap[0].size());
    int startPosRow = getGridRowIndex(startPos.z, baseDimensions.z, heightMap.size());
    int endPosCol = getGridColumnIndex(endPos.x, baseDimensions.x, heightMap[0].size());
    int endPosRow = getGridRowIndex(endPos.z, baseDimensions.z, heightMap.size());

    // Calculate the radius in grid space
    int gridRadiusX = getGridRadius(radius, baseDimensions.x, heightMap[0].size());
    int gridRadiusZ = getGridRadius(radius, baseDimensions.z, heightMap.size());

    // Process each point around the milling start position
    for (int i = startPosCol - gridRadiusX; i <= startPosCol + gridRadiusX; i++)
    {
        for (int j = startPosRow - gridRadiusZ; j <= startPosRow + gridRadiusZ; j++)
        {
            int col = i;
            int row = j;
            if (isWithinBounds(col, row, heightMap))
            {
                glm::vec3 gridPosition = getGridPosition(col, row, baseDimensions, heightMap);
                float pointDistanceSquared = computeSquaredDistance(gridPosition.x, startPos.x, gridPosition.z,
                                                                    startPos.z);

                // Check if the point is within the milling radius
                if (pointDistanceSquared < radius * radius)
                {
                    float pointHeight = computeMilledHeight(pointDistanceSquared, radius, startPos.y, type);

                    // Check if the point's height should be milled
                    if (heightMap[col][row] > pointHeight)
                    {
                        // Check if the milling path is too steep
                        if (descendAngle > maxDescendAngle && type == Flat)
                        {
                            return std::unexpected("Mill path too steep");
                        }

                        // Check if the milling tool is cutting with the non-cutting part
                        if (heightMap[col][row] - pointHeight > height)
                        {
                            return std::unexpected("Milling with a non-cutting part");
                        }

                        // Update the height map safely
                        updateHeightMap(heightMap, col, row, pointHeight);
                    }
                }
            }
        }
    }

    // Initialize error tracking variables
    bool error = false;
    std::string errorMessage = "";

    // Calculate the main milling direction vector
    glm::vec3 direction = endPos - startPos;

    // Compute the perpendicular vector to the milling path in the XZ-plane
    glm::vec3 perpendicular = glm::normalize(glm::cross(glm::normalize(direction), glm::vec3(0, 1, 0)));

    // Define the middle pixel (2D grid coordinates) based on the starting position
    glm::vec<2, int> pixelMiddle = glm::vec<2, int>(startPosCol, startPosRow);

    // Define the bottom and top pixels by offsetting along the perpendicular direction
    glm::vec<2, int> pixelBottom = pixelMiddle - glm::vec<2, int>(perpendicular.x * gridRadiusX,
                                                                  perpendicular.z * gridRadiusZ);
    glm::vec<2, int> pixelTop = pixelMiddle + glm::vec<2, int>(perpendicular.x * gridRadiusX,
                                                               perpendicular.z * gridRadiusZ);

    // Use Bresenham's line algorithm to iterate over the line between the bottom and top pixels
    bresenham(pixelBottom.x, pixelBottom.y, pixelTop.x, pixelTop.y, [&](int x, int y, float str)
    {
        // Convert grid coordinates (x, y) to 3D world space
        glm::vec3 gridPosition = getGridPosition(x, y, baseDimensions, heightMap);
        float pointRadius2 = computeSquaredDistance(gridPosition.x, startPos.x, gridPosition.z, startPos.z);

        // Define the difference vector (2D) between start and end grid positions
        glm::vec<2, int> diff = {endPosCol - startPosCol, endPosRow - startPosRow};

        // Use Bresenham's line algorithm to mill along the direction from (x, y) to the end point
        bresenham(x, y, x + diff.x, y + diff.y, [&](int x, int y, float str)
        {
            // Check if the point is within the bounds of the height map
            if (!isWithinBounds(x, y, heightMap)) { return; }

            // Convert inner grid coordinates (x, y) to 3D world space
            glm::vec3 gridPositionInner = getGridPosition(x, y, baseDimensions, heightMap);

            // Calculate vector AB (milling direction) and vector AP (from grid position to current point)
            auto AB = glm::vec2(direction.x, direction.z);
            auto AP = glm::vec2(gridPositionInner.x, gridPositionInner.z) - glm::vec2(gridPosition.x, gridPosition.z);

            // Calculate the projection of AP onto AB using dot product (for linear interpolation)
            float lengthSqrAB = AB.x * AB.x + AB.y * AB.y;
            float t = (AP.x * AB.x + AP.y * AB.y) / lengthSqrAB;

            // If t > 1, the current point is beyond the end position
            if (t > 1)
                return;

            // Calculate the interpolated height along the milling path
            float pointHeight = (startPos + t * direction).y;

            // Adjust height based on the milling type (Flat or Spherical)
            pointHeight = computeMilledHeight(pointRadius2, radius, pointHeight, type);

            // If the height map at (x, y) is higher than the computed point height, modify the height map
            if (heightMap[x][y] > pointHeight)
            {
                // Check if the descend angle is too steep for Flat milling, which would cause errors
                if (descendAngle > maxDescendAngle && type == Flat)
                {
                    errorMessage = "Mill path too steep";
                    return;
                }

                // Ensure that the milling tool isn't cutting with the non-cutting part
                if (heightMap[x][y] - pointHeight > height)
                {
                    error = true;
                    errorMessage = "Milling with a non-cutting part";
                    return;
                }

                // Safely update the height map with mutex locking for thread safety
                updateHeightMap(heightMap, x, y, pointHeight);
            }
        });
    });

    // If an error occurred during the milling process, return the error message
    if (error)
        return std::unexpected(errorMessage);

    // Return success if no errors occurred
    return {};
}

// Helper function to calculate grid column index from X-coordinate
int Mill::getGridColumnIndex(float xCoord, float baseWidth, int mapWidth)
{
    return (xCoord + baseWidth / 2.0f) / baseWidth * mapWidth;
}

// Helper function to calculate grid row index from Z-coordinate
int Mill::getGridRowIndex(float zCoord, float baseDepth, int mapDepth)
{
    return (zCoord + baseDepth / 2.0f) / baseDepth * mapDepth;
}

// Helper function to calculate the grid radius
int Mill::getGridRadius(float radius, float baseDimension, int mapSize)
{
    return (radius / baseDimension) * mapSize + 2;
}

// Helper function to check if the grid indices are within bounds
bool Mill::isWithinBounds(int col, int row, const std::vector<std::vector<float> > &heightMap)
{
    return col >= 0 && col < heightMap[0].size() && row >= 0 && row < heightMap.size();
}

// Helper function to calculate the grid position in 3D space
glm::vec3 Mill::getGridPosition(int col, int row, glm::vec3 baseDimensions,
                                const std::vector<std::vector<float> > &heightMap)
{
    return glm::vec3(
        float(col) / heightMap[0].size() * baseDimensions.x - baseDimensions.x / 2.0f,
        0.0f,
        float(row) / heightMap.size() * baseDimensions.z - baseDimensions.z / 2.0f
    );
}

// Helper function to compute the squared distance between two points
float Mill::computeSquaredDistance(float x1, float x2, float z1, float z2)
{
    return std::pow(x1 - x2, 2) + std::pow(z1 - z2, 2);
}

// Helper function to compute the height based on tool type (Flat or Spherical)
float Mill::computeMilledHeight(float pointRadius2, float radius, float startHeight, MillType type)
{
    float pointHeight = startHeight;
    if (type == Spherical) { pointHeight += radius - sqrt(radius * radius - pointRadius2); }
    return pointHeight;
}

// Helper function to update the height map with locking for thread safety
void Mill::updateHeightMap(std::vector<std::vector<float> > &heightMap, int col, int row, float newHeight)
{
    heightMapMutex.lock();
    heightMap[col][row] = newHeight;
    heightMapMutex.unlock();
}
