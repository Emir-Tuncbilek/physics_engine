//
// Created by Emir Tuncbilek on 1/7/25.
//

#ifndef PHYSICS_ENGINE_TEXTURE2D_H
#define PHYSICS_ENGINE_TEXTURE2D_H


#include <GL/glew.h>
#include <iostream>

class Texture2D {
public:
    Texture2D(const char * filePath);
    ~Texture2D();
    void use();
    void unuse();

private:
    GLuint id;
};


#endif //PHYSICS_ENGINE_TEXTURE2D_H
