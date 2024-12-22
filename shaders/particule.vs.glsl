#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 inTexCoords;
layout (location = 2) in vec3 normals;

uniform mat4 U_MVP;


void main() {
    const vec3 velocity = vec3(1.0f, 0.0f, 0.0f);
    const float stretchFactor = 0.05;
    vec3 stretchedPosition = position + (normalize(velocity) * stretchFactor * length(velocity));
    gl_Position = U_MVP * vec4(position.xyz, 1.0f);
}