//
// Created by Emir Tuncbilek on 8/30/24.
//

#ifndef PHYSICS_ENGINE_MODELS_H
#define PHYSICS_ENGINE_MODELS_H

#pragma once
#include <GL/glew.h>
#include <iostream>
#include <vector>

#include "./shapes.h"

class Model {
public:
    explicit Model(const char* path);
    void draw();
    std::vector<GLfloat> vertexData;
private:
    static void loadObj(const char* path, std::vector<GLfloat>& vertexData, std::vector<GLuint>& indices);

private:
    BasicShapeElements m_shape;
    GLsizei m_count;
};

#endif //PHYSICS_ENGINE_MODELS_H
