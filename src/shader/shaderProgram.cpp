//
// Created by Bartek Jadczak on 03/03/2024.
//

#include "shaderProgram.h"

#include <cerrno>
#include <fstream>
#include <iostream>
#include<glm/gtc/type_ptr.hpp>


// Reads a text file and outputs a string with everything in the text file
std::string get_file_contents(const char* filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size()); // NOLINT(*-narrowing-conversions)
        in.close();
        return(contents);
    }
    throw(errno);
}

// Constructor that build the Shader Program from 2 different shaders
ShaderProgram::ShaderProgram(
        const char* vertexFile,
        const char* fragmentFile,
        const char* tessellationEvaluationFile,
        const char* tessellationControlFile) :
        vertexFile(vertexFile),
        fragmentFile(fragmentFile),
        tessellationEvaluationFile(tessellationEvaluationFile),
        tessellationControlFile(tessellationControlFile)
{
    setUpShader(ID);
}

// Activates the Shader Program
void ShaderProgram::Activate()
{
    glUseProgram(ID);
}

// Deletes the Shader Program
void ShaderProgram::Delete()
{
    glDeleteProgram(ID);
}

void ShaderProgram::setUniform(const std::string &name, ShaderType &&value, unsigned long size) const {
    std::visit(overloaded{
                   [&](bool &v) { glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) v); },
                   [&](int &v) { glUniform1i(glGetUniformLocation(ID, name.c_str()), v); },
                   [&](float &v) { glUniform1f(glGetUniformLocation(ID, name.c_str()), v); },
                   [&](glm::vec3 &v) { glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &v[0]); },
                   [&](glm::vec4 &v) { glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &v[0]); },
                   [&](glm::mat4 &v) { glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &v[0][0]); },
                   [&](glm::vec3* &v) {
                       auto *buffer = new float[size*3];
                       for(int i = 0; i < size; i++) {
                           buffer[i*3+0] = v[i].x;
                           buffer[i*3+1] = v[i].y;
                           buffer[i*3+2] = v[i].z;
                       }
                       glUniform3fv(glGetUniformLocation(ID, name.c_str()), GLsizei(size), buffer);
                   }
               }, value);
}

// Checks if the different Shaders have compiled properly
bool ShaderProgram::compileErrors(unsigned int shader, const char* type)
{
    // Stores status of compilation
    GLint hasCompiled;
    // Character array to store error message in
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
            return false;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
            return false;
        }
    }

    return true;
}

void ShaderProgram::createShader (GLuint &shader, const char *source, GLenum shaderType, const char *type)
{
    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    compileErrors(shader, type);
}

bool ShaderProgram::setUpShader (GLuint &shaderProgramID)
{
    // Read vertexFile and fragmentFile and store the strings
    std::string vertexCode = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);

    // Convert the shader source strings into character arrays
    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    GLuint vertexShader;
    createShader(vertexShader, vertexSource, GL_VERTEX_SHADER, "VERTEX");

    GLuint fragmentShader;
    createShader(fragmentShader, fragmentSource, GL_FRAGMENT_SHADER, "FRAGMENT");

    GLuint TES;
    if(tessellationEvaluationFile)
    {
        std::string TESCode = get_file_contents(tessellationEvaluationFile);
        const char* TESSource = TESCode.c_str();
        createShader(TES, TESSource, GL_TESS_EVALUATION_SHADER, "TES");
    }

    GLuint TCS;
    if(tessellationControlFile)
    {
        std::string TCSCode = get_file_contents(tessellationControlFile);
        const char* TCSSource = TCSCode.c_str();
        createShader(TCS, TCSSource, GL_TESS_CONTROL_SHADER, "TCS");
    }

    // Create Shader Program Object and get its reference
    shaderProgramID = glCreateProgram();
    // Attach the Vertex and Fragment Shaders to the Shader Program
    glAttachShader(shaderProgramID, vertexShader);
    glAttachShader(shaderProgramID, fragmentShader);
    if(tessellationEvaluationFile) glAttachShader(shaderProgramID, TES);
    if(tessellationControlFile) glAttachShader(shaderProgramID, TCS);

    // Wrap-up/Link all the shaders together into the Shader Program
    glLinkProgram(shaderProgramID);
    // Checks if Shaders linked succesfully
    bool ret = compileErrors(shaderProgramID, "PROGRAM");

    // Delete the now useless Vertex and Fragment Shader objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if(tessellationEvaluationFile) glDeleteShader(TES);
    if(tessellationControlFile) glDeleteShader(TCS);

    return ret;
}

void ShaderProgram::reloadFromFile ()
{
    GLuint tmpNewID;
    if(setUpShader(tmpNewID))
    {
        this->Delete();
        ID = tmpNewID;
    }
}