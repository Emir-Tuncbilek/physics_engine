//
// Created by Emir Tuncbilek on 8/30/24.
//

#include "shapes.h"

BasicShapeElements::BasicShapeElements() {
    glGenVertexArrays(1, &this->m_vao);
    glGenBuffers(1, &this->m_vbo);
    glGenBuffers(1, &this->m_ebo);
    glBindVertexArray(this->m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ebo);

}

BasicShapeElements::BasicShapeElements(const GLfloat* data, GLsizeiptr byteSize, const GLuint* indexes, GLsizeiptr indexesByteSize) {
    glGenVertexArrays(1, &this->m_vao);
    glBindVertexArray(this->m_vao);
    glGenBuffers(1, &this->m_vbo);
    glGenBuffers(1, &m_ebo);

    this->setData(data, byteSize, indexes, indexesByteSize);
}

BasicShapeElements::~BasicShapeElements() {
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &this->m_vbo);
    glDeleteBuffers(1, &this->m_ebo);
    glDeleteVertexArrays(1, &this->m_vao);
}

void BasicShapeElements::setData(const GLfloat* data, GLsizeiptr byteSize, const GLuint* indexes, GLsizeiptr indexesByteSize) {

    glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
    glBufferData(GL_ARRAY_BUFFER, byteSize, data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexesByteSize, indexes, GL_STATIC_DRAW);
}

void BasicShapeElements::enableAttribute(GLuint index, GLint size, GLsizei stride, GLsizeiptr offset) {
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)(offset * sizeof(GLfloat)));
    glEnableVertexAttribArray(index);
}

void BasicShapeElements::draw(GLenum mode, GLsizei count) const {
    glBindVertexArray(this->m_vao);
    glDrawElements(mode, count, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
