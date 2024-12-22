//
// Created by Emir Tuncbilek on 8/30/24.
//

#ifndef PHYSICS_ENGINE_SHAPES_H
#define PHYSICS_ENGINE_SHAPES_H

#pragma once
#include <GL/glew.h>
#include <iostream>
#include <vector>

class BasicShapeArrays
{
public:
    BasicShapeArrays();
    BasicShapeArrays(const GLfloat* data, GLsizeiptr byteSize);
    ~BasicShapeArrays();

    void enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset);
    void draw(GLenum mode, GLsizei count);

private:
    GLuint m_vao;
    GLuint m_vbo;
};

class BasicShapeElements {
public:
    BasicShapeElements();
    BasicShapeElements(const GLfloat* data, GLsizeiptr byteSize, const GLuint* indexes, GLsizeiptr indexesByteSize);
    ~BasicShapeElements();

    void setData(const GLfloat* data, GLsizeiptr byteSize, const GLuint* indexes, GLsizeiptr indexesByteSize);

    void enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset);
    void draw(GLenum mode, GLsizei count) const;

private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
};

#endif //PHYSICS_ENGINE_SHAPES_H
