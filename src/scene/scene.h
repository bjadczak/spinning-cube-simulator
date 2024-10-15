//
// Created by Bartosz Jadczak on 09/10/2024.
//

#ifndef SCENE_H
#define SCENE_H

#include "../app/appContext.h"
#include "../renderContext/renderContext.h"
#include "../gridModule/gridModule.h"

class Scene {
    AppContext &appContext;
    RenderContext &renderContext;

    std::unique_ptr<GridModule> grid;

    std::unique_ptr<ShaderProgram> millingShader;
    std::unique_ptr<ShaderProgram> basicShader;

public:
    explicit Scene(AppContext &appContext, RenderContext &renderContext);
    void update();
    void render();
    void destroy();
};



#endif //SCENE_H
