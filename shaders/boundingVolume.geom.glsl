#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 normal[];
out vec3 barycentric;
out vec3 geomNormal;

void main() {
    gl_Position = gl_in[0].gl_Position;
    barycentric = vec3(1.0f, 0.0f, 0.0f);
    geomNormal = normal[0];
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    barycentric = vec3(0.0f, 1.0f, 0.0f);
    geomNormal = normal[1];
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    barycentric = vec3(0.0f, 0.0f, 1.0f);
    geomNormal = normal[2];
    EmitVertex();

    EndPrimitive();
}