#version 330 core

in vec2 texCoords;
in vec3 fragPos;
in vec3 normal;

uniform sampler2D textureSampler;

out vec4 FragColor;

void main() {
    vec4 texel = texture(textureSampler, texCoords);
    if (texel.a < 0.05) discard;
    FragColor = texel;
}