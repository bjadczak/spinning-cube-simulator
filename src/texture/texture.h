//
// Created by Bartosz Jadczak on 10/10/2024.
//

#ifndef TEXTURE_H
#define TEXTURE_H



class Texture {
    unsigned int id{};
    int nrChannels;
    int target, format, internalFormat;

public:
    Texture(int x, int y, int nrChannels, int internalFormat, int format, int type, int target, void* data);

    void bind(int slot) const;
    void bindImage(int slot) const;
    void update2D(void* data) const;
    void updatePixel(int x, int y, void* data) const;
    unsigned int getID() const;
    int width;
    int height;
    int depth;
    int type;
};



#endif //TEXTURE_H
