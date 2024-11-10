//
// Created by Bartosz Jadczak on 09/10/2024.
//

#include "scene.h"

Scene::Scene(AppContext &appContext, RenderContext &renderContext) :
    appContext(appContext),
    renderContext(renderContext)
{
    basicShader = std::make_unique<ShaderProgram>(
    "res/shaders/basic/basic.vert",
    "res/shaders/basic/basic.frag"
    );
    grid = std::make_unique<GridModule>(appContext);

    appContext.cube = std::make_unique<Cube>();
    appContext.cubeSimulation = std::make_unique<CubeSimulation>();
    appContext.lastFrameTimeMs = glfwGetTime();
}

void Scene::update() {
    if(true/*appContext.running*/) {
        float timeMs = glfwGetTime() * 1000;
        int loopsToDo = static_cast<int>((timeMs - appContext.lastFrameTimeMs) / appContext.cubeSimulation->timeStepMs);
        appContext.lastFrameTimeMs += loopsToDo * appContext.cubeSimulation->timeStepMs;
        for (int i = 0; i < loopsToDo; i++) {
            appContext.cubeSimulation->advanceByStep();
        }
        appContext.cubeSimulation->updateTrace();
    } else {
        appContext.lastFrameTimeMs = glfwGetTime() * 1000.f;
    }
}

void Scene::render() {
    appContext.frameBufferManager->bind();

    appContext.cubeSimulation->renderLine(*basicShader, *appContext.camera);
    appContext.cubeSimulation->renderGravityLine(*basicShader, *appContext.camera);

    grid->draw();

    appContext.cubeSimulation->renderCube(*basicShader, *appContext.camera, *appContext.cube);


    appContext.frameBufferManager->unbind();
}

void Scene::destroy() {
    grid->destroy();
    basicShader->Delete();
}
