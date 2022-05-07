#version 400 core

layout (location = 0) in vec3 a_position; // The vertex coordinates

uniform mat4 u_matrix; // The transformation matrix

void main() {
    gl_Position = u_matrix * vec4(a_position, 1.0); // Calculate the vertex position in the clip space
}
