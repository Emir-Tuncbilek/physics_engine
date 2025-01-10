//
// Created by Emir Tuncbilek on 1/7/25.
//

#include "Texture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include "./public-libs/stb_image.h"


Texture2D::Texture2D(const char *filePath) {
    int width, height, nChannels;
    stbi_set_flip_vertically_on_load(true);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    unsigned char* data = stbi_load(filePath, &width, &height, &nChannels, 0);
    if (data == nullptr)
        std::cout << "Error loading texture \"" << filePath << "\": " << stbi_failure_reason() << std::endl;

    glGenTextures(1, &this->id);
    glBindTexture(GL_TEXTURE_2D, this->id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLenum format = GL_RGBA;
    switch (nChannels)
    {
        case 1: format = GL_RED;  break;
        case 2: format = GL_RG;   break;
        case 3: format = GL_RGB;  break;
        case 4: format = GL_RGBA; break;
        default: break;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, (int)format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}

Texture2D::~Texture2D() {
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &this->id);
}

void Texture2D::use() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->id);
}

void Texture2D::unuse() {
    glBindTexture(GL_TEXTURE_2D, 0);
}