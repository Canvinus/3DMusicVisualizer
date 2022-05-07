#version 400 core

layout (location = 0) in vec4 a_vertex; // The vertex and texture coordinates

uniform mat4 u_projectionMatrix; // The projection matrix

out vec2 v_textureCoordinates; // The texture coordinates

void main() {
    gl_Position = u_projectionMatrix * vec4(a_vertex.xy, 0.0, 1.0); // Calculate the vertex position in the clip space
    v_textureCoordinates = a_vertex.zw; // Pass the texture coordinates
}