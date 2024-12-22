#version 330 core

// in vec2 texCoords;
in vec3 fragPos;
in vec3 normal;

// uniform sampler2D textureSampler;

out vec4 FragColor;

void main() {
    vec3 color = vec3(0.75f) - 0.75f * normal;
    vec3 colorWeights = vec3(color.r * 0.3, color.g * 0.59, color.b * 0.11);    // Greyscaled
    float newColor = (colorWeights.r + colorWeights.g + colorWeights.b);
    // vec4 texel = vec4(vec3(0.5f) - 0.25 * normal, 1.0f); // just white for now // texture(textureSampler, texCoords);
    // FragColor = vec4(texel.rgb, 1.0f);
    FragColor = vec4(vec3(newColor), 1.0f);
}