//
// Created by Bartosz Jadczak on 09/11/2024.
//

#ifndef CUBE_H
#define CUBE_H
#include <memory>
#include "../mesh/mesh.h"


class Cube {
    std::unique_ptr<Mesh> objectMesh;

public:
    explicit Cube();

    void render();

    void update();
};



#endif //CUBE_H
