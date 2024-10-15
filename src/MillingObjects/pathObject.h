//
// Created by Bartosz Jadczak on 15/10/2024.
//

#ifndef PATHOBJECT_H
#define PATHOBJECT_H

#include <memory>
#include "../mesh/mesh.h"

class PathObject {
    std::unique_ptr<Mesh> objectMesh;



public:
    explicit PathObject(const std::vector<glm::vec3>& path);
    void generateMesh(const std::vector<glm::vec3>& path);
    void render() const;
};



#endif //PATHOBJECT_H
