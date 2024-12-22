//
// Created by Emir Tuncbilek on 8/29/24.
//

#include "textures.h"
#define STB_IMAGE_IMPLEMENTATION
#include "public-libs/stb_image.h"

Texture2D::Texture2D(const char* path, GLenum wrapMode) {
    int width, height, nChannels;
    stbi_set_flip_vertically_on_load(true);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    unsigned char* data = stbi_load(path, &width, &height, &nChannels, 0);
    if (data == nullptr)
        std::cerr << "Error loading texture \"" << path << "\": " << stbi_failure_reason() << std::endl;

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

    GLenum format = GL_RGBA;
    switch (nChannels) {
        case 1: format = GL_RED;  break;
        case 2: format = GL_RG;   break;
        case 3: format = GL_RGB;  break;
        case 4: format = GL_RGBA; break;
        default: break;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}

Texture2D::~Texture2D() {
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &m_id);
}

void Texture2D::enableMipmap() const {
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture2D::use(GLuint unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture2D::unuse() {
    glBindTexture(GL_TEXTURE_2D, 0);
}