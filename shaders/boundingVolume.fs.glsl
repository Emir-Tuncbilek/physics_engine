#version 330 core

// in vec2 texCoords;
in vec3 fragPos;
in vec3 normal;

// uniform sampler2D textureSampler;

out vec4 FragColor;

void main() {
    vec3 color = vec3(0.75f, 0.25f, 0.25f);
    FragColor = vec4(vec3(color), 1.0f);
}