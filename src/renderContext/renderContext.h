//
// Created by Bartosz Jadczak on 08/10/2024.
//

#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#define GL_SILENCE_DEPRECATION
#include "../glew_glfw.h"
#include "../app/appContext.h"

class RenderContext {
    AppContext &appContext;
    GLFWwindow* window;
    const char* glsl_version;

public:

    explicit RenderContext(AppContext &appContext);
    ~RenderContext();

    static void pollWindowEvents() { glfwPollEvents(); }
    void swapBuffers() const { glfwSwapBuffers(window); }
    void clearWindow() const;
    [[nodiscard]] bool shouldWindowClose() const { return glfwWindowShouldClose(window); }
    [[nodiscard]] GLFWwindow* getWindow() const { return window; }
    [[nodiscard]] const char *getGLSLVersion() const { return glsl_version; }

};



#endif //RENDERCONTEXT_H
