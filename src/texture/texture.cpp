//
// Created by Bartosz Jadczak on 10/10/2024.
//

#include "texture.h"
#include "../glew_glfw.h"

Texture::Texture(int x, int y, int nrChannels, int internalFormat, int format, int type, int target, void* data) {
    width = x;
    height = y;
    this->type = type;
    this->format = format;
    this->internalFormat = internalFormat;
    this->target = target;
    this->nrChannels = nrChannels;

    glGenTextures(1, &id);
    glBindTexture(target, id);

    // Upload the initial data to the GPU
    glTexImage2D(target, 0, internalFormat, x, y, 0, format, type, data);

    // Set texture parameters
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Texture::bind(int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(target, id);
}

void Texture::bindImage(int slot) const {
    glBindTextureUnit(slot, id);
    glBindImageTexture(slot, id, 0, GL_FALSE, 0, GL_READ_WRITE, internalFormat);
}

void Texture::update2D(void *data) const {
    // Update the texture
    glBindTexture(target, id);
    glTexSubImage2D(target, 0, 0, 0, width, height, format, GL_FLOAT, data);
}

void Texture::updatePixel(int x, int y, void *data) const {
    glBindTexture(target, id);
    glTexSubImage2D(
            target,
            0,
            x,
            y,
            1,
            1,
            format,
            GL_FLOAT,
            data
    );
}

unsigned int Texture::getID() const {
    return id;
}
