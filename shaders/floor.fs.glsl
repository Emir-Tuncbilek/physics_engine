#version 330 core

// in vec2 texCoords;
in vec3 fragPos;
in vec3 normal;

// uniform sampler2D textureSampler;

out vec4 FragColor;

void main() {
    vec3 color = vec3(0.9f, 0.1f, 0.1f);
    FragColor = vec4(color, 1.0f);
}