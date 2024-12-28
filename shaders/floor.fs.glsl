#version 330 core

// in vec2 texCoords;
in vec3 fragPos;
in vec3 normal;

// uniform sampler2D textureSampler;

out vec4 FragColor;

void main() {
    vec3 colorNormal = vec3(0.75f) - 0.75f * normal;
    vec3 color = vec3(0.9f * colorNormal.r, 0.1f * colorNormal.g, 0.1f * colorNormal.b);
    FragColor = vec4(color, 1.0f);
}