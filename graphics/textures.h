//
// Created by Emir Tuncbilek on 8/29/24.
//

#ifndef PHYSICS_ENGINE_TEXTURES_H
#define PHYSICS_ENGINE_TEXTURES_H

#include <GL/glew.h>
#include <iostream>


class Texture2D {
public:
    Texture2D(const char* path, GLenum wrapMode);
    ~Texture2D();

    void enableMipmap() const;

    void use(GLuint unit = 0) const;
    static void unuse();

private:
    GLuint m_id;
};

#endif //PHYSICS_ENGINE_TEXTURES_H
