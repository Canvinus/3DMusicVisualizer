#version 400 core

layout (location = 0) in vec3 a_position; // The vertex coordinates
layout (location = 1) in vec3 a_normal; // The vertex normal for lighting

uniform vec3 u_forward; // The forward direction of the camera for lighting
uniform vec3 u_color; // The object color
uniform mat4 u_normalMatrix; // The normal matrix
uniform mat4 u_matrix; // The transformation matrix

out vec3 v_color; // The color that is passed to the fragment shader and interpolated

void main() {
    vec3 normal = normalize(mat3(u_normalMatrix) * a_normal); // Transform the normal
    float lighting = 0.2 + 0.8 * max(-dot(normal, u_forward), 0.0); // Calculate lighting as a sum of ambient and directional lighting
    v_color = lighting * u_color; // Calculate the vertex color based on lighting
    gl_Position = u_matrix * vec4(a_position, 1.0); // Calculate the vertex position in the clip space
}
