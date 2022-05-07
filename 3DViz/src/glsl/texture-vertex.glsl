#version 400 core

layout (location = 0) in vec3 a_position; // The vertex coordinates
layout (location = 1) in vec3 a_normal; // The vertex normal for lighting
layout (location = 2) in vec2 a_textureCoordinates; // The texture coordinates

uniform mat4 u_modelMatrix; // The model matrix
uniform mat4 u_normalMatrix; // The normal matrix
uniform mat4 u_matrix; // The transformation matrix

out vec3 v_position; // The vertex position in the world space
out vec3 v_normal; // The vertex normal for lighting
out vec2 v_textureCoordinates; // The texture coordinates

void main() {
    v_position = vec3(u_modelMatrix * vec4(a_position, 1.0)); // Calculate the vertex position in the world space
    v_normal = normalize(mat3(u_normalMatrix) * a_normal); // Transform the normal
    v_textureCoordinates = a_textureCoordinates; // Pass the texture coordinates
    gl_Position = u_matrix * vec4(a_position, 1.0); // Calculate the vertex position in the clip space
}
