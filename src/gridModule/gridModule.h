//
// Created by Bartosz Jadczak on 09/10/2024.
//

#ifndef GRIDMODULE_H
#define GRIDMODULE_H

#include <memory>
#include "../app/appContext.h"
#include "../shader/shaderProgram.h"
#include "../mesh/mesh.h"
#include "../glew_glfw.h"



class GridModule {
    AppContext &appContext;
public:
    explicit GridModule (AppContext &appContext);

    void draw() const;
    void destroy() const;

private:
    std::unique_ptr<Mesh> objectMesh;
    std::unique_ptr<ShaderProgram> shader;

};



#endif //GRIDMODULE_H
