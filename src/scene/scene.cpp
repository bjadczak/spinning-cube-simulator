//
// Created by Bartosz Jadczak on 09/10/2024.
//

#include "scene.h"

Scene::Scene(AppContext &appContext, RenderContext &renderContext) :
    appContext(appContext),
    renderContext(renderContext)
{
    grid = std::make_unique<GridModule>(appContext);
}

void Scene::update() {
}

void Scene::render() {
    appContext.frameBufferManager->bind();

    grid->draw();
    appContext.frameBufferManager->unbind();
}

void Scene::destroy() {
    grid->destroy();
}
