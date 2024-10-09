//
// Created by Bartek Jadczak on 07/05/2024.
//

#ifndef CAMERAGAMELIKE_H
#define CAMERAGAMELIKE_H


#include "baseCamera.h"

class CameraGameLike : public BaseCamera
{
public:
    explicit CameraGameLike(
            int screenWidth,
            int screenHeight,
            CameraMode cameraMode = FREE,
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 anchor = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::quat orientation = glm::quat(1, 0, 0, 0)
    );

    [[nodiscard]] glm::mat4 getViewMatrix () const override;
    [[nodiscard]] glm::mat4 getNoTranslationViewMatrix () const override;
    [[nodiscard]] glm::mat4 getMirrorViewMatrix () const override;
    [[nodiscard]] glm::mat4 getProjectionMatrix () const override;
    [[nodiscard]] glm::vec3 getViewPosition () const override;
    [[nodiscard]] float getNearPlane() const override;
    [[nodiscard]] float getFarPlane() const override;

    void processKeyboard (CameraMovement direction, float deltaTime) override;
    void processMouseMovement (float xoffset, float yoffset) override;
    void processMouseScroll (float yoffset) override;
    void resize (int screenWidth, int screenHeight) override;

private:
    // Stores the main vectors of the camera
    glm::vec3 Position;
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 cameraMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 projectionMatrix = glm::mat4(1.0f);

    float FOVdeg;
    float nearPlane;
    float farPlane;

    // Prevents the camera from jumping around when first clicking left click
    bool firstClick = true;

    // Adjust the speed of the camera and it's sensitivity when looking around
    const float lowSpeed = 0.01f;
    const float highSpeed = 0.1f;
    float speed = lowSpeed;
    float sensitivity = 100.0f;

    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void speedUp();
    void speedDown();
    void rotate(float rotX, float rotY);

    void resetLocation();
};


#endif //CAMERAGAMELIKE_H
