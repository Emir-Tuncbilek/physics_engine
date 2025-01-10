#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 inTexCoords;
layout (location = 2) in vec3 normals;


out vec2 texCoords;

uniform mat4 U_MVP;

out vec3 vertexPosOut;
out vec3 normal;

void main() {
    vec4 pos = vec4(U_MVP * vec4(position.xyz, 1.0));
    vertexPosOut = vec3(pos.xyz);
    normal = normals;
    texCoords = inTexCoords;
    gl_Position = pos;
}
