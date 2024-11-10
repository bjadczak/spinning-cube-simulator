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
}

void Scene::update() {
}

void Scene::render() {
    appContext.frameBufferManager->bind();

    basicShader->Activate();
    basicShader->setUniform("model", glm::mat4(1.0f));
    basicShader->setUniform("projection", appContext.camera->getProjectionMatrix());
    basicShader->setUniform("view", appContext.camera->getViewMatrix());
    appContext.cube->render();

    grid->draw();
    appContext.frameBufferManager->unbind();
}

void Scene::destroy() {
    grid->destroy();
    basicShader->Delete();
}
