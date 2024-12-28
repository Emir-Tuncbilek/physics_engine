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
    void draw() const;
    std::vector<GLfloat> vertexData;

private:
    static void loadObj(const char* path, std::vector<GLfloat>& vertexData, std::vector<GLuint>& indices);

private:
    GLsizei m_count;
    BasicShapeElements m_shape;
};

#endif //PHYSICS_ENGINE_MODELS_H
