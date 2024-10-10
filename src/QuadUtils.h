//
// Created by Bartosz Jadczak on 09/10/2024.
//

#ifndef QUADUTILS_H
#define QUADUTILS_H

#include <vector>
#include "glew_glfw.h"

inline void addQuad(std::vector<GLuint> &indices, const GLuint i0, const GLuint i1, const GLuint i2, const GLuint i3) {
    indices.push_back(i0);
    indices.push_back(i1);
    indices.push_back(i2);

    indices.push_back(i3);
    indices.push_back(i0);
    indices.push_back(i2);
}

#endif //QUADUTILS_H
