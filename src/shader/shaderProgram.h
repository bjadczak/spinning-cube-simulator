//
// Created by Bartek Jadczak on 03/03/2024.
//

#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <string>
#include <variant>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "../glew_glfw.h"
#include "../VariantUtil.h"

std::string get_file_contents(const char* filename);

using ShaderType = std::variant<bool, int, float, glm::vec3, glm::vec4, glm::mat4, glm::vec3*>;

class ShaderProgram
{
public:
    // Reference ID of the Shader Program
    GLuint ID;
    // Constructor that build the Shader Program from 2 different shaders
    ShaderProgram(
            const char* vertexFile,
            const char* fragmentFile,
            const char* tessellationEvaluationFile = nullptr,
            const char* tessellationControlFile = nullptr);

    // Activates the Shader Program
    void Activate();
    // Deletes the Shader Program
    void Delete();

    // Set Uniform to be sent to shader
    // Make sure you pass the correct type to the function! For example value "42" will be treated as int even if the uniform is float. Use "42.f" instead.
    void setUniform(const std::string &name, ShaderType &&value, unsigned long size = 1) const;;

    void reloadFromFile();

private:
    const char* vertexFile;
    const char* fragmentFile;
    const char* tessellationEvaluationFile;
    const char* tessellationControlFile;

    bool setUpShader(GLuint &shaderProgramID);

    // Checks if the different Shaders have compiled properly
    bool compileErrors(unsigned int shader, const char* type);

    void createShader(GLuint& shader, const char* source, GLenum shaderType, const char* type);
};


#endif // SHADERPROGRAM_H
