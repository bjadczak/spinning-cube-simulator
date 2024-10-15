//
// Created by Bartek Jadczak on 07/05/2024.
//

#include "CameraGameLike.h"
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/gtc/reciprocal.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

static glm::mat4 lookAtMatrix(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
    auto f(normalize(center - eye));
    auto s(normalize(cross(f, up)));
    auto u(cross(s, f));

    return {
            {s.x, u.x, -f.x, 0},
            {s.y, u.y, -f.y, 0},
            {s.z, u.z, -f.z, 0},
            {-dot(s, eye), -dot(u, eye), dot(f, eye), 1},
    };
}

static glm::mat4 myProjectionMatrix(float fovy, float aspect, float zNear, float zFar)
{
    float const tanHalfFovy = tan(fovy / 2);

    glm::mat4 Result(0);
    Result[0][0] = 1 / (aspect * tanHalfFovy);
    Result[1][1] = 1 / (tanHalfFovy);
    Result[2][2] = - (zFar + zNear) / (zFar - zNear);
    Result[2][3] = - 1;
    Result[3][2] = - (2 * zFar * zNear) / (zFar - zNear);
    return Result;
}

void CameraGameLike::resize (int screenWidth, int screenHeight)
{
    // Update width and height
    CameraGameLike::screenWidth = screenWidth;
    CameraGameLike::screenHeight = screenHeight;
}

glm::mat4 CameraGameLike::getViewMatrix () const
{
    return lookAtMatrix(Position, Position + Orientation, Up);
}

glm::mat4 CameraGameLike::getNoTranslationViewMatrix () const
{
    return glm::mat3(lookAtMatrix(Position, Position + Orientation, Up));
}

glm::mat4 CameraGameLike::getMirrorViewMatrix () const
{
    glm::mat4 mirrorModel(1.0f);

    mirrorModel = glm::rotate(mirrorModel, glm::pi<float>()/6, glm::vec3(0,0,1));
    mirrorModel = glm::translate(mirrorModel, glm::vec3{-1.23f, 1.0f, 0.0f});
    mirrorModel = glm::scale(mirrorModel, glm::vec3(2,2,2));
    return  lookAtMatrix(Position, Position + Orientation, Up) * mirrorModel * glm::scale(glm::mat4(1), glm::vec3(-1, 1, 1)) * glm::inverse(mirrorModel);
}

glm::mat4 CameraGameLike::getProjectionMatrix () const
{
    return myProjectionMatrix(FOVdeg, static_cast<float>(screenWidth) / static_cast<float>(screenHeight), nearPlane, farPlane);
}

glm::vec3 CameraGameLike::getViewPosition () const
{
    return Position;
}

float CameraGameLike::getNearPlane() const {
    return nearPlane;
}

float CameraGameLike::getFarPlane() const {
    return farPlane;
}

void CameraGameLike::processKeyboard (CameraMovement direction, float deltaTime)
{
    if (direction == FORWARD)
        moveForward();
    if (direction == BACKWARD)
        moveBackward();
    if (direction == LEFT)
        moveLeft();
    if (direction == RIGHT)
        moveRight();
    if (direction == UP)
        moveUp();
    if (direction == DOWN)
        moveDown();
    if (direction == SPEEDUP)
        speedUp();
    if (direction == SPEEDDOWN)
        speedDown();
}

void CameraGameLike::processMouseMovement (float xoffset, float yoffset)
{
    rotate(-xoffset, -yoffset);
}

void CameraGameLike::processMouseScroll (float yoffset)
{
    // Nothing
}

CameraGameLike::CameraGameLike (
        int screenWidth, int screenHeight, CameraMode cameraMode, glm::vec3 position, glm::vec3 anchor,
        glm::quat orientation
) :
        Position(position),
        FOVdeg(glm::pi<float>() / 4.0f),
        nearPlane(1.f),
        farPlane(1000.0f)
{
    BaseCamera::type = CameraType::GAMELIKE;
    BaseCamera::screenWidth = screenWidth;
    BaseCamera::screenHeight = screenHeight;
}

void CameraGameLike::moveForward ()
{
    Position += speed * +Orientation;
}

void CameraGameLike::moveBackward ()
{
    Position += speed * -Orientation;
}

void CameraGameLike::moveLeft ()
{
    Position += speed * -glm::normalize(glm::cross(Orientation, Up));
}

void CameraGameLike::moveRight ()
{
    Position += speed * glm::normalize(glm::cross(Orientation, Up));
}

void CameraGameLike::moveUp ()
{
    Position += speed * Up;
}

void CameraGameLike::moveDown ()
{
    Position += speed * -Up;
}

void CameraGameLike::speedUp ()
{
    speed = highSpeed;
}

void CameraGameLike::speedDown ()
{
    speed = lowSpeed;
}

void CameraGameLike::rotate(float rotX, float rotY)
{
    // Calculates upcoming vertical change in the Orientation
    glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(rotX), glm::normalize(glm::cross(Orientation, Up)));

    // Decides whether or not the next vertical Orientation is legal or not
    if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
    {
        Orientation = newOrientation;
    }

    // Rotates the Orientation left and right
    Orientation = glm::rotate(Orientation, glm::radians(rotY), Up);
}

void CameraGameLike::resetLocation ()
{
    Position = glm::vec3(0.0f, 0.0f, 2.0f);
    Orientation = glm::vec3(0.0f, 0.0f, 1.0f);
    Up = glm::vec3(0.0f, 1.0f, 0.0f);
}