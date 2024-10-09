//
// Created by Bartosz Jadczak on 09/10/2024.
//

#include "InputHandler.h"

void InputHandler::setupCallbacks() {
    glfwSetWindowUserPointer(renderContext.getWindow(), this);

    glfwSetWindowSizeCallback(renderContext.getWindow(), [](GLFWwindow *window, int width, int height) {
        auto *inputHandler = static_cast<InputHandler *>(glfwGetWindowUserPointer(window));
        inputHandler->windowResize(window, width, height);
    });
}

void InputHandler::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    if(appContext.cameraType == CameraType::FREEANCHOR)
    {
        appContext.camera->processMouseScroll(static_cast<float>(yoffset));
    }
}

void InputHandler::windowResize (GLFWwindow *window, int width, int height)
{
    appContext.glfw_window_resize(width, height);
}
