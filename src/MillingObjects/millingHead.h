//
// Created by Bartosz Jadczak on 22/10/2024.
//

#ifndef MILLINGHEAD_H
#define MILLINGHEAD_H

#include <vector>
#include <memory>
#include "../mesh/mesh.h"

struct Vertex;

enum MillType {
    Flat,
    Spherical
};

class MillingHead {
    MillType type;
    float height;
    float radius;

    std::unique_ptr<Mesh> cylinderMesh;

public:
    MillingHead();

    void setType(MillType type);
    void setHeight(float height);
    void setRadius(float radius);

    void generateMillingHead();

    static void generateCylinder(float radius, float height, int segments, MillType type,
                                                           std::vector<Vertex>& vertices,
                                                           std::vector<unsigned int>& indices);

    void render();
};



#endif //MILLINGHEAD_H
