//
// Created by Bartosz Jadczak on 10/10/2024.
//

#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include <glm/vec3.hpp>

#include "../shader/shaderProgram.h"

struct PointLight {

    glm::vec3 position = glm::vec3(0, 1.0f, 0);
    glm::vec3 color = glm::vec3(255/255.f, 214/255.f, 170/255.f);
    float strength = 10.f;
    float constantAttenuation = 10.0f;
    float linearAttenuation = 7.f;
    float quadraticAttenuation = 18.f;
    int chosenLight = 1;

    void setupPointLight(ShaderProgram& shader) {
        shader.setUniform("pointLight.position", position);
        shader.setUniform("pointLight.color", color);
        shader.setUniform("pointLight.strength", strength);
        shader.setUniform("pointLight.constantAttenuation", constantAttenuation);
        shader.setUniform("pointLight.linearAttenuation", linearAttenuation);
        shader.setUniform("pointLight.quadraticAttenuation", quadraticAttenuation);
    }
};

#endif //POINTLIGHT_H
