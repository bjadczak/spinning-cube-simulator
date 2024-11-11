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
    appContext.allocateCubeSimulation(CubeSimulationType::THREAD);
    appContext.lastFrameTimeMs = glfwGetTime();
}

void Scene::update() {
    if(appContext.isRunning) {
        float timeMs = glfwGetTime() * 1000;
        int loopsToDo = static_cast<int>((timeMs - appContext.lastFrameTimeMs) / appContext.cubeSimulation->timeStepMs);
        appContext.lastFrameTimeMs += loopsToDo * appContext.cubeSimulation->timeStepMs;
        appContext.cubeSimulation->addLoopsToDo(loopsToDo);
        appContext.cubeSimulation->updateTrace();
    } else {
        appContext.lastFrameTimeMs = glfwGetTime() * 1000.f;
    }
}

void Scene::render() {
    appContext.frameBufferManager->bind();

    if(appContext.cubeSimulation != nullptr)
    {
        basicShader->Activate();
        appContext.cubeSimulation->renderLine(*basicShader, *appContext.camera, appContext.showLine);
        appContext.cubeSimulation->renderGravityLine(*basicShader, *appContext.camera, appContext.showGravity);
        appContext.cubeSimulation->renderCube(*basicShader, *appContext.camera, *appContext.cube, appContext.showCube, appContext.showDiagonal);

    }

    grid->draw();

    appContext.frameBufferManager->unbind();
}

void Scene::destroy() {
    appContext.cubeSimulation->stopThread();
    grid->destroy();
    basicShader->Delete();
}
