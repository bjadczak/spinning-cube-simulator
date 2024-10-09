//
// Created by Bartek Jadczak on 03/03/2024.
//

#ifndef EBO_H
#define EBO_H

#include <vector>
#include "../glew_glfw.h"

class EBO
{
public:
    // ID reference of Elements Buffer Object
    GLuint ID;
    // Constructor that generates a Elements Buffer Object and links it to indices
    EBO(const std::vector<GLuint>& indices);

    // Binds the EBO
    void Bind();
    // Unbinds the EBO
    void Unbind();
    // Deletes the EBO
    void Delete();
};


#endif //EBO_H
