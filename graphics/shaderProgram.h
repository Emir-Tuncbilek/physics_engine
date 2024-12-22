//
// Created by Emir Tuncbilek on 8/29/24.
//

#ifndef PHYSICS_ENGINE_SHADERPROGRAM_H
#define PHYSICS_ENGINE_SHADERPROGRAM_H

#include <GL/glew.h>

class Shader {
public:
    Shader(GLenum type, const char* code);
    ~Shader();

    GLuint id();

private:
    void checkError();

private:
    GLuint m_id;
};


class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();

    void use();
    void attachShader(Shader& s);
    void link();

    GLint getUniformLoc(const char* name);
    void setUniformBlockBinding(const char* name, GLuint bindingIndex);

    void setTransformFeedbackVaryings(const char** names, GLsizei number, GLenum mode);

    GLuint getID() const { return this->m_id; }

private:
    void checkError();

private:
    GLuint m_id;
};

#endif //PHYSICS_ENGINE_SHADERPROGRAM_H
