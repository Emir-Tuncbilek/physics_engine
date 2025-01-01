#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 inTexCoords;
layout (location = 2) in vec3 normals;


// out vec2 texCoords;

uniform mat4 U_MVP;

out vec3 normal;

void main() {
    normal = normals;
    gl_Position = vec4(U_MVP * vec4(position.xyz, 1.0));
}
