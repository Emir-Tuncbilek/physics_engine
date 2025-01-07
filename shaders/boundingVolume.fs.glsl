#version 330 core

// in vec2 texCoords;
in vec3 fragPos;
in vec3 normal;
in vec3 barycentric;    // Barycentric coordinates passed from the geom shader

out vec4 FragColor;

void main() {
    const float edgeThreshold = 0.05;
    vec3 color = vec3(0.75f, 0.25f, 0.25f);
    if (barycentric.x < edgeThreshold || barycentric.y < edgeThreshold || barycentric.z < edgeThreshold) {
        FragColor = vec4(vec3(color), 1.0f);
        return;
    }
    discard;
}