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

inline void addQuad(std::vector<GLuint> &indices, const GLuint i0, const GLuint i1, const GLuint i2, const GLuint i3) {
    indices.push_back(i0);
    indices.push_back(i1);
    indices.push_back(i2);

    indices.push_back(i3);
    indices.push_back(i0);
    indices.push_back(i2);
}

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
