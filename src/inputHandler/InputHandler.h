//
// Created by Bartosz Jadczak on 09/10/2024.
//

#ifndef PUMAROBOT_INPUTHANDLER_H
#define PUMAROBOT_INPUTHANDLER_H

#include <functional>
#include "../scene/scene.h"

class InputHandler {
    AppContext &appContext;
    RenderContext &renderContext;
public:
    explicit InputHandler(AppContext &appContext, RenderContext &renderContext) :
        appContext(appContext), renderContext(renderContext) {}

    void setupCallbacks();

private:
    void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) const;
    void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    void mouseCallback(GLFWwindow *window, double xposIn, double yposIn);
    void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    void windowResize(GLFWwindow* window, int width, int height);
};




#endif //PUMAROBOT_INPUTHANDLER_H
