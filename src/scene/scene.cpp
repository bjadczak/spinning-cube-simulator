//
// Created by Bartosz Jadczak on 09/10/2024.
//

#include "scene.h"
#include <memory>
#include "../MillingObjects/mill.h"
#include "../MillingObjects/millingPlate.h"

Scene::Scene(AppContext &appContext, RenderContext &renderContext) :
    appContext(appContext),
    renderContext(renderContext)
{
    grid = std::make_unique<GridModule>(appContext);
    millingShader = std::make_unique<ShaderProgram>(
        "res/shaders/millingObject/millingObject.vert",
        "res/shaders/millingObject/millingObject.frag",
        "res/shaders/millingObject/millingObject.geom"
        );
    basicShader = std::make_unique<ShaderProgram>(
    "res/shaders/basic/basic.vert",
    "res/shaders/basic/basic.frag"
    );

    appContext.light = std::make_unique<PointLight>();
    appContext.light->position = {0.0f , 0.0f, 0.25f};

    appContext.baseDimensions = {150, 50, 150};
    appContext.baseResolution = {1500, 1500};

    appContext.millingObject = std::make_unique<MillingPlate>(appContext.baseResolution, appContext.baseDimensions.y);
    appContext.mill = std::make_unique<Mill>(40, 8, glm::vec3(0, 60, 0), 300, 0.999, 10);
}

void Scene::update() {
    if(appContext.mill->isThreadRunning() ) {
        appContext.millingObject->update();
    }
}

void Scene::render() {
    appContext.frameBufferManager->bind();

    millingShader->Activate();
    millingShader->setUniform("model", glm::mat4(1.0f));
    millingShader->setUniform("viewPos", appContext.camera->getViewPosition());
    millingShader->setUniform("view", appContext.camera->getViewMatrix());
    millingShader->setUniform("projection", appContext.camera->getProjectionMatrix());
    millingShader->setUniform("material.hasTexture", false);
    millingShader->setUniform("material.albedo", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
    millingShader->setUniform("material.shininess", 256.f);
    millingShader->setUniform("uGridSize", appContext.baseResolution);
    millingShader->setUniform("uBaseSize", glm::vec2(appContext.baseDimensions.x,appContext.baseDimensions.z));
    millingShader->setUniform("uHeightScale", appContext.baseDimensions.y);
    appContext.millingObject->heightMap->bind(1);
    millingShader->setUniform("uHeightMap", 1);
    appContext.light->setupPointLight(*millingShader);
    appContext.millingObject->render((appContext.baseResolution.x + 2) * (appContext.baseResolution.y + 2));

    basicShader->Activate();
    basicShader->setUniform("model", glm::mat4(1.0f));
    basicShader->setUniform("projection", appContext.camera->getProjectionMatrix());
    basicShader->setUniform("view", appContext.camera->getViewMatrix());
    if(appContext.pathObject != nullptr) {

        appContext.pathObject->render();
    }

    appContext.mill->render(*basicShader);

    grid->draw();
    appContext.frameBufferManager->unbind();
}

void Scene::destroy() {
    grid->destroy();
    millingShader->Delete();
}
