//
// Created by Bartosz Jadczak on 10/11/2024.
//

#ifndef LINE_H
#define LINE_H
#include <memory>
#include "../mesh/mesh.h"


class Line {
    std::unique_ptr<Mesh> lineMesh;

public:
    Line();

    void updatePoints(std::vector<glm::vec3> points);

    void render();
};



#endif //LINE_H
