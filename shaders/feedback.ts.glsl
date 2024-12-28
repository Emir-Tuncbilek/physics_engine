#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

out vec3 vertexPos;

void main() {
    vertexPos = position;
    gl_Position = vec4(position, 1.0);
}