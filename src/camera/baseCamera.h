//
// Created by Bartek Jadczak on 07/05/2024.
//

#ifndef BASECAMERA_H
#define BASECAMERA_H

#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <map>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    SPEEDUP,
    SPEEDDOWN
};

enum CameraMode {
    FREE,
    ANCHOR
};

enum CameraType {
    FREEANCHOR,
    GAMELIKE,
    COUNT
};

// Default camera values
const float SPEED = 2.5f;
const float SENSITIVITY = 0.01f;
const float ZOOM_SENSITIVITY = 0.1f;

class BaseCamera
{
public:
    CameraType type;
    [[nodiscard]] virtual glm::mat4 getViewMatrix() const = 0;
    [[nodiscard]] virtual glm::mat4 getNoTranslationViewMatrix() const = 0;
    [[nodiscard]] virtual glm::mat4 getMirrorViewMatrix() const = 0;
    [[nodiscard]] virtual glm::mat4 getProjectionMatrix() const = 0;
    [[nodiscard]] virtual glm::vec3 getViewPosition() const = 0;
    [[nodiscard]] virtual float getNearPlane() const = 0;
    [[nodiscard]] virtual float getFarPlane() const = 0;

    virtual void processKeyboard(CameraMovement direction, float deltaTime) = 0;
    virtual void processMouseMovement(float xoffset, float yoffset) = 0;
    virtual void processMouseScroll(float yoffset) = 0;

    virtual void resize(int screenWidth, int screenHeight) = 0;

    int screenWidth;
    int screenHeight;
};


#endif //BASECAMERA_H
