//
// Created by Emir Tuncbilek on 8/29/24.
//

#include "shaderProgram.h"

#include <iostream>

#define MSG_SIZE 1024

Shader::Shader(GLenum type, const char* code) {
    m_id = glCreateShader(type);
    glShaderSource(m_id, 1, &code, nullptr);
    glCompileShader(m_id);
    checkError();
}

Shader::~Shader() { glDeleteShader(m_id); }

GLuint Shader::id() { return m_id; }

void Shader::checkError() {
    GLint success;
    GLchar infoLog[1024];

    glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(m_id, 1024, NULL, infoLog);
        glDeleteShader(m_id);
        std::cout << "Shader compile error: " << infoLog << std::endl;
    }
}

ShaderProgram::ShaderProgram() { m_id = glCreateProgram(); }

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(m_id);
}

void ShaderProgram::use() {
    glUseProgram(m_id);
}

void ShaderProgram::attachShader(Shader& s) {
    glAttachShader(m_id, s.id());
}

void ShaderProgram::link() {
    glLinkProgram(m_id);
    checkError();
}

GLint ShaderProgram::getUniformLoc(const char* name) {
    return glGetUniformLocation(m_id, name);
}

void ShaderProgram::setUniformBlockBinding(const char* name, GLuint bindingIndex) {
    GLuint blockIndex = glGetUniformBlockIndex(m_id, name);
    glUniformBlockBinding(m_id, blockIndex, bindingIndex);
}

void ShaderProgram::setTransformFeedbackVaryings(const char** names, GLsizei number, GLenum mode){
    glTransformFeedbackVaryings(m_id, number, names, mode);
}

void ShaderProgram::checkError() {
    GLint success;
    GLchar infoLog[MSG_SIZE];

    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_id, MSG_SIZE, nullptr, infoLog);
        glDeleteProgram(m_id);
        std::cout << "Program linking error: " << infoLog << std::endl;
    }
}
